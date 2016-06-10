#import "QuipTextView.h"

#import "QuipStatusView.h"

#include "AttributeRange.hpp"
#include "Color.hpp"
#include "EditContext.hpp"
#include "Key.hpp"
#include "KeyStroke.hpp"
#include "Mode.hpp"
#include "Selection.hpp"
#include "Syntax.hpp"

#include <cmath>
#include <map>
#include <vector>

namespace {
  struct Highlight {
    CFDictionaryRef attributes;
    CGColorRef foregroundColor;
  };

  static void initializeHighlight (Highlight * highlight, quip::Color foreground) {
      highlight->foregroundColor = CGColorCreateGenericRGB(foreground.r, foreground.g, foreground.b, foreground.a);
      
      CFStringRef keys[] = { kCTForegroundColorAttributeName };
      CFTypeRef values[] = { highlight->foregroundColor };
      const void ** opaqueKeys = reinterpret_cast<const void **>(&keys);
      const void ** opaqueValues = reinterpret_cast<const void **>(&values);
      highlight->attributes = CFDictionaryCreate(kCFAllocatorDefault, opaqueKeys, opaqueValues, 1, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  }

  static void releaseHighlight (Highlight * highlight) {
    CFRelease(highlight->attributes);
    CFRelease(highlight->foregroundColor);
  }
}

@interface QuipTextView () {
@private
  CTFontRef m_font;
  CFDictionaryRef m_fontAttributes;
  CTParagraphStyleRef m_paragraphAttributes;
  std::vector<CTTextTabRef> m_tabStops;
  
  Highlight m_highlightAttributes[quip::AttributeCount];
  
  CGSize m_cellSize;
  CGRect m_minimumFrame;
  
  CGFloat m_cursorTimer;
  BOOL m_isCursorVisible;
  
  std::shared_ptr<quip::EditContext> m_context;
  
  QuipStatusView * m_statusView;
}
@end

static NSString * gSizeQueryString = @"m";

static std::size_t gTabSize = 2;

static CGFloat gTickInterval = 1.0 / 30.0;
static CGFloat gCursorBlinkInterval = 0.57;

@implementation QuipTextView

- (instancetype)initWithFrame:(NSRect)frame {
  self = [super initWithFrame:frame];
  if (self != nil) {
    m_font = CTFontCreateWithName(CFSTR("Menlo"), 13.0, nil);
    
    NSDictionary * attributes = @{NSFontAttributeName: [NSFont fontWithName:@"Menlo" size:13.0f]};
    m_cellSize = [gSizeQueryString sizeWithAttributes:attributes];
    m_minimumFrame = frame;
    
    std::size_t tabStopCount = static_cast<std::size_t>(std::ceil(frame.size.width / m_cellSize.width));
    m_tabStops.reserve(tabStopCount);
    for (std::size_t index = 0; index < tabStopCount; ++index) {
      m_tabStops.emplace_back(CTTextTabCreate(kCTTextAlignmentNatural, (index + 1) * m_cellSize.width * gTabSize, nullptr));
    };
    
    CFArrayRef tabStopArray = CFArrayCreate(kCFAllocatorDefault, reinterpret_cast<const void **>(m_tabStops.data()), tabStopCount, &kCFTypeArrayCallBacks);
    CTParagraphStyleSetting paragraphSettings[] = {
      { kCTParagraphStyleSpecifierTabStops, sizeof(CFArrayRef), &tabStopArray },
    };
    
    m_paragraphAttributes = CTParagraphStyleCreate(paragraphSettings, 1);
    
    CFStringRef keys[] = { kCTFontAttributeName, kCTParagraphStyleAttributeName };
    CFTypeRef values[] = { m_font, m_paragraphAttributes };
    const void ** opaqueKeys = reinterpret_cast<const void **>(&keys);
    const void ** opaqueValues = reinterpret_cast<const void **>(&values);
    m_fontAttributes = CFDictionaryCreate(kCFAllocatorDefault, opaqueKeys, opaqueValues, 2, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    initializeHighlight(m_highlightAttributes + quip::Keyword, quip::Color(0.0f, 0.0f, 1.0f));
    initializeHighlight(m_highlightAttributes + quip::Preprocessor, quip::Color(0.5f, 0.25f, 0.1f));
    initializeHighlight(m_highlightAttributes + quip::Comment, quip::Color(0.0f, 0.5f, 0.0f));
    
    m_context = nullptr;
    m_statusView = nullptr;
    
    m_cursorTimer = gCursorBlinkInterval;
    m_isCursorVisible = YES;
    
    // Start a background timer for periodic update tasks (such as cursor blinking).
    NSTimer * timer = [NSTimer timerWithTimeInterval:gTickInterval target:self selector:@selector(tick:) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
  }
  
  return self;
}

- (void)dealloc {
  for (std::size_t index = 0; index < quip::AttributeCount; ++index) {
    releaseHighlight(m_highlightAttributes + index);
  }
  
  CFRelease(m_paragraphAttributes);
  for (CTTextTabRef & tab : m_tabStops) {
    CFRelease(tab);
  }
  
  CFRelease(m_fontAttributes);
  CFRelease(m_font);
}

- (BOOL)acceptsFirstResponder {
  return YES;
}

- (BOOL)validateMenuItem:(NSMenuItem *)menuItem {
  if([menuItem action] == @selector(performUndo:)) {
    return m_context->canUndo();
  }
  
  if([menuItem action] == @selector(performRedo:)) {
    return m_context->canRedo();
  }
  
  return YES;
}

- (void)performUndo:(id)sender {
  m_context->undo();
}

- (void)performRedo:(id)sender {
  m_context->redo();
}

- (void)tick:(NSTimer *)timer {
  m_cursorTimer -= gTickInterval;
  if (m_cursorTimer <= 0.0) {
    m_cursorTimer = gCursorBlinkInterval;
    m_isCursorVisible = !m_isCursorVisible;
    
    [self setNeedsDisplay:YES];
  }
}

- (void)resetCursorBlink {
  m_cursorTimer = gCursorBlinkInterval;
  m_isCursorVisible = YES;
  
  [self setNeedsDisplay:YES];
}

- (void)mouseDown:(NSEvent *)event {
  NSPoint location = [self convertPoint:event.locationInWindow fromView:nil];
  std::size_t column = static_cast<std::size_t>(location.x / m_cellSize.width);
  std::size_t row = static_cast<std::size_t>((self.frame.size.height - location.y) / m_cellSize.height);
  quip::Location target(column, row);
  
  if (target.row() >= m_context->document().rows() || target.column() >= m_context->document().row(target.row()).length()) {
    return;
  }
  
  m_context->selections().replace(quip::Selection(target));
  [self resetCursorBlink];
}

- (void)mouseDragged:(NSEvent *)event {
  NSPoint location = [self convertPoint:event.locationInWindow fromView:nil];
  std::size_t column = static_cast<std::size_t>(location.x / m_cellSize.width);
  std::size_t row = static_cast<std::size_t>((self.frame.size.height - location.y) / m_cellSize.height);
  quip::Location target(column, row);
  
  if (target.row() >= m_context->document().rows() || target.column() >= m_context->document().row(target.row()).length()) {
    return;
  }
  
  quip::Location origin = m_context->selections().primary().origin();
  m_context->selections().replace(quip::Selection(origin, target));
  [self resetCursorBlink];
}

- (void)keyDown:(NSEvent *)event {
  quip::KeyStroke keyStroke(static_cast<quip::Key>(event.keyCode), std::string([[event characters] cStringUsingEncoding:NSUTF8StringEncoding]));
  if (m_context->processKey(keyStroke)) {
    [self resetCursorBlink];
  }
}

- (void)cut:(id)sender {
  NSPasteboard * pasteboard = [NSPasteboard generalPasteboard];
  [pasteboard clearContents];
  
  NSMutableArray * items = [[NSMutableArray alloc] initWithCapacity:m_context->selections().count()];
  for (const quip::Selection & selection : m_context->selections()) {
    std::string text = m_context->document().contents(selection);
    [items addObject:[NSString stringWithUTF8String:text.c_str()]];
  }
  
  m_context->selections().replace(m_context->document().erase(m_context->selections()));
  [pasteboard writeObjects:items];
}

- (void)copy:(id)sender {
  NSPasteboard * pasteboard = [NSPasteboard generalPasteboard];
  [pasteboard clearContents];
  
  NSMutableArray * items = [[NSMutableArray alloc] initWithCapacity:m_context->selections().count()];
  for (const quip::Selection & selection : m_context->selections()) {
    std::string text = m_context->document().contents(selection);
    [items addObject:[NSString stringWithUTF8String:text.c_str()]];
  }
  
  [pasteboard writeObjects:items];
}

- (void)paste:(id)sender {
  NSPasteboard * pasteboard = [NSPasteboard generalPasteboard];
  NSArray * items = [pasteboard readObjectsForClasses:@[[NSString class]] options:@{}];
  if (items != nil) {
    NSString * item = [items firstObject];
    std::string text = [item cStringUsingEncoding:NSUTF8StringEncoding];
    m_context->selections().replace(m_context->document().insert(m_context->selections(), text));
  }
}

- (void)selectAll:(id)sender {
  const quip::Document & document = m_context->document();
  std::uint64_t row = document.rows() - 1;
  std::uint64_t column = document.row(row).size() - 1;
  
  quip::Selection selection(quip::Location(0, 0), quip::Location(column, row));
  m_context->selections().replace(selection);
}

- (quip::Document &)document {
  return m_context->document();
}

- (void)setDocument:(std::shared_ptr<quip::Document>)document {
  m_context = std::make_shared<quip::EditContext>(document);

  CGFloat frameWidth = m_minimumFrame.size.width;
  CGFloat frameHeight = std::max(m_minimumFrame.size.height, m_cellSize.height * document->rows());
  [self setFrameSize:CGSizeMake(frameWidth, frameHeight)];
  [self setNeedsDisplay:YES];
  
  // Bind controller signals.
  m_context->controller().scrollToLocation.connect([=] (quip::Location location) {
    [self scrollToLocation:location];
  });
}

- (void)setStatus:(QuipStatusView *)status {
  m_statusView = status;
}

- (void)scrollToLocation:(quip::Location)location {
  CGFloat y = self.frame.size.height - (m_cellSize.height * (location.row() + 1));
  
  // The parent of this view is the NSClipView, which will have the height of the
  // visible portion of the document, which can be used to bias the target point
  // to the center of the view.
  CGFloat bias = self.superview.frame.size.height * 0.5;
  [self scrollPoint:CGPointMake(0.0, y - bias)];
}

- (void)drawSelections:(const quip::SelectionDrawInfo &)drawInfo context:(CGContextRef)context {
  quip::Document & document = m_context->document();
  for (const quip::Selection & selection : drawInfo.selections) {
    const quip::Location & lower = selection.lowerBound();
    const quip::Location & upper = selection.upperBound();
    std::size_t row = lower.row();
    
    do {
      std::size_t firstColumn = row == lower.row() ? lower.column() : 0;
      std::size_t lastColumn = row == upper.row() ? upper.column() : document.row(row).size() - 1;
      
      CGFloat x = firstColumn * m_cellSize.width;
      CGFloat y = self.frame.size.height - m_cellSize.height - (row * m_cellSize.height);
      const quip::Color & color = selection == drawInfo.selections.primary() ? drawInfo.primaryColor : drawInfo.secondaryColor;
      CGContextSetRGBStrokeColor(context, color.r, color.g, color.b, color.a);
      CGContextSetRGBFillColor(context, color.r, color.g, color.b, color.a);
      
      if (m_isCursorVisible || (drawInfo.flags & quip::CursorFlags::Blink) == 0) {
        switch (drawInfo.style) {
          case quip::CursorStyle::VerticalBlock:
            CGContextFillRect(context, CGRectMake(x, y - 2.0, m_cellSize.width * (lastColumn + 1 - firstColumn), 0.75 * m_cellSize.height));
            break;
          case quip::CursorStyle::VerticalBlockHalf:
            CGContextFillRect(context, CGRectMake(x, y - 2.0, m_cellSize.width * (lastColumn + 1 - firstColumn), 0.25 * m_cellSize.height));
            break;
          case quip::CursorStyle::VerticalBar:
            CGContextMoveToPoint(context, x, y - 2.0);
            CGContextAddLineToPoint(context, x, y + m_cellSize.height - 6.0);
            CGContextStrokePath(context);
            break;
          case quip::CursorStyle::Underline:
          default:
            CGContextMoveToPoint(context, x, y - 2.0);
            CGContextAddLineToPoint(context, x + (m_cellSize.width * (lastColumn + 1 - firstColumn)), y - 2.0);
            CGContextStrokePath(context);
            break;
        }
      }
      
      ++row;
    } while (row <= upper.row());
  }
}

- (void)drawRect:(NSRect)dirtyRect {
  quip::Document & document = m_context->document();
  CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
  
  // Clear the background.
  CGContextSetRGBFillColor(context, 1.0f, 1.0f, 1.0f, 1.0f);
  CGContextFillRect(context, dirtyRect);
  
  // Draw selections and overlays first (text is drawn over them).
  if ([[self window] isKeyWindow]) {
    NSColor * systemHighlightColor = [[NSColor selectedTextBackgroundColor] colorUsingColorSpaceName:NSCalibratedRGBColorSpace];
    quip::Color primaryColor([systemHighlightColor redComponent], [systemHighlightColor greenComponent], [systemHighlightColor blueComponent]);
    quip::Color secondaryColor(primaryColor.r * 0.5f, primaryColor.g * 0.5f, primaryColor.b * 0.5f);
    
    quip::SelectionDrawInfo drawInfo;
    drawInfo.primaryColor = primaryColor;
    drawInfo.secondaryColor = secondaryColor;
    drawInfo.flags = m_context->mode().cursorFlags();
    drawInfo.style = m_context->mode().cursorStyle();
    drawInfo.selections = m_context->selections();
    [self drawSelections:drawInfo context:context];
  }
  
  for (auto && overlay : m_context->overlays()) {
    [self drawSelections:overlay.second context:context];
  }
  
  // Draw text.
  CGFloat y = self.frame.size.height - m_cellSize.height;
  for (std::size_t row = 0; row < document.rows(); ++row) {
    // Only draw the row if it clips into the dirty rectangle.
    if (CGRectIntersectsRect(dirtyRect, CGRectMake(0.0, y, self.frame.size.width, m_cellSize.height))) {
      std::vector<quip::AttributeRange> syntaxAttributes = m_context->document().highlight(row);
      CFStringRef text = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, document.row(row).c_str(), kCFStringEncodingUTF8, kCFAllocatorNull);
      CFMutableAttributedStringRef attributed = CFAttributedStringCreateMutable(kCFAllocatorDefault, CFStringGetLength(text));

      CFAttributedStringBeginEditing(attributed);
      CFAttributedStringReplaceString(attributed, CFRangeMake(0, 0), text);
      CFAttributedStringSetAttributes(attributed, CFRangeMake(0, CFStringGetLength(text)), m_fontAttributes, YES);
      
      for (const quip::AttributeRange & range : syntaxAttributes) {
        CFAttributedStringSetAttributes(attributed, CFRangeMake(range.start, range.length), m_highlightAttributes[range.name].attributes, NO);
      }
      
      CFAttributedStringEndEditing(attributed);
      
      CTLineRef line = CTLineCreateWithAttributedString(attributed);
      CGContextSetTextPosition(context, 0.0f, y);
      CTLineDraw(line, context);
      
      CFRelease(line);
      CFRelease(attributed);
      CFRelease(text);
    }
    
    y -= m_cellSize.height;
  }
  
  [m_statusView setStatus:m_context->mode().status().c_str()];
  [m_statusView setLineCount:m_context->document().rows()];
  [m_statusView setFileType:"?"];
}

@end
