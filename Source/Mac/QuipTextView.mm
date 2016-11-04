#import "QuipTextView.h"

#import "QuipPopupView.h"
#import "QuipStatusView.h"

#include "AttributeRange.hpp"
#include "ChangeType.hpp"
#include "Color.hpp"
#include "DrawingServiceProvider.hpp"
#include "EditContext.hpp"
#include "Extent.hpp"
#include "Key.hpp"
#include "Mode.hpp"
#include "PopupServiceProvider.hpp"
#include "Selection.hpp"
#include "StatusServiceProvider.hpp"
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
    highlight->foregroundColor = CGColorCreateGenericRGB(foreground.r(), foreground.g(), foreground.b(), foreground.a());
    
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
  
  CGFloat m_cursorTimer;
  BOOL m_shouldDrawCursor;
  BOOL m_shouldDrawSelections;

  std::unique_ptr<quip::DrawingService> m_drawingServiceProvider;
  std::unique_ptr<quip::PopupServiceProvider> m_popupServiceProvider;
  std::unique_ptr<quip::StatusServiceProvider> m_statusServiceProvider;
  std::shared_ptr<quip::EditContext> m_context;
  
  QuipStatusView * m_statusView;
  
  std::uint32_t m_scrollToLocationToken;
  std::uint32_t m_scrollLocationIntoViewToken;
  std::uint32_t m_documentModifiedToken;
  std::uint32_t m_transactionAppliedToken;
}
@end

static NSString * gSizeQueryString = @"m";

static std::size_t gTabSize = 2;
static CGFloat gMargin = 1.0f;

static CGFloat gTickInterval = 1.0 / 30.0;
static CGFloat gCursorBlinkInterval = 0.57;

@implementation QuipTextView

- (instancetype)initWithFrame:(NSRect)frame {
  self = [super initWithFrame:frame];
  if (self != nil) {
    m_drawingServiceProvider = std::make_unique<quip::DrawingServiceProvider>("Menlo", 13.0f);
    m_popupServiceProvider = std::make_unique<quip::PopupServiceProvider>(self);
    
    m_font = CTFontCreateWithName(CFSTR("Menlo"), 13.0, nil);
    
    quip::Extent cellSize = m_drawingServiceProvider->cellSize();
    std::size_t tabStopCount = static_cast<std::size_t>(std::ceil(frame.size.width / cellSize.width()));
    m_tabStops.reserve(tabStopCount);
    for (std::size_t index = 0; index < tabStopCount; ++index) {
      m_tabStops.emplace_back(CTTextTabCreate(kCTTextAlignmentNatural, (index + 1) * cellSize.width() * gTabSize, nullptr));
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
    m_shouldDrawCursor = YES;
    m_shouldDrawSelections = YES;
    
    m_scrollToLocationToken = 0;
    m_scrollLocationIntoViewToken = 0;
    m_documentModifiedToken = 0;
    m_transactionAppliedToken = 0;
    
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
  
  m_popupServiceProvider.reset();
}

- (void)setFrame:(NSRect)frame {
  [super setFrame:frame];
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
  [self setNeedsDisplay:YES];
}

- (void)performRedo:(id)sender {
  m_context->redo();
  [self setNeedsDisplay:YES];
}

- (void)tick:(NSTimer *)timer {
  m_popupServiceProvider->tick(gTickInterval);
  
  m_cursorTimer -= gTickInterval;
  if (m_cursorTimer <= 0.0) {
    m_cursorTimer = gCursorBlinkInterval;
    m_shouldDrawCursor = !m_shouldDrawCursor;
    
    [self setNeedsDisplay:YES];
  }
}

- (void)resetCursorBlink {
  m_cursorTimer = gCursorBlinkInterval;
  m_shouldDrawCursor = YES;
  
  [self setNeedsDisplay:YES];
}

- (void)mouseDown:(NSEvent *)event {
  NSPoint location = [self convertPoint:event.locationInWindow fromView:nil];
  quip::Coordinate coordinate(location.x, location.y);
  quip::Rectangle rectangle(self.frame.origin.x, self.frame.origin.y, self.frame.size.width, self.frame.size.height);
  quip::Location target = m_drawingServiceProvider->locationForCoordinateInFrame(coordinate, rectangle);
  
  if (target.row() >= m_context->document().rows() || target.column() >= m_context->document().row(target.row()).length()) {
    return;
  }
  
  m_context->selections().replace(quip::Selection(target));
  [self resetCursorBlink];
}

- (void)mouseDragged:(NSEvent *)event {
  NSPoint location = [self convertPoint:event.locationInWindow fromView:nil];
  quip::Coordinate coordinate(location.x, location.y);
  quip::Rectangle rectangle(self.frame.origin.x, self.frame.origin.y, self.frame.size.width, self.frame.size.height);
  quip::Location target = m_drawingServiceProvider->locationForCoordinateInFrame(coordinate, rectangle);
  
  if (target.row() >= m_context->document().rows() || target.column() >= m_context->document().row(target.row()).length()) {
    return;
  }
  
  quip::Location origin = m_context->selections().primary().origin();
  m_context->selections().replace(quip::Selection(origin, target));
  [self resetCursorBlink];
}

- (void)keyDown:(NSEvent *)event {
  quip::Key key = quip::keyFromScanCode(event.keyCode);
  std::string text = key == quip::Key::Return ? "\n" : std::string([[event characters] cStringUsingEncoding:NSUTF8StringEncoding]);
  if (m_context->processKeyEvent(key, text)) {
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
  if (m_context != nullptr) {
    m_context->controller().scrollToLocation.disconnect(m_scrollToLocationToken);
    m_scrollToLocationToken = 0;
    
    m_context->controller().scrollLocationIntoView.disconnect(m_scrollLocationIntoViewToken);
    m_scrollLocationIntoViewToken = 0;
    
    m_context->document().onDocumentModified().disconnect(m_documentModifiedToken);
    m_documentModifiedToken = 0;
    
    m_context->onTransactionApplied().disconnect(m_transactionAppliedToken);
    m_transactionAppliedToken = 0;
  }
  
  m_context = std::make_shared<quip::EditContext>(m_popupServiceProvider.get(), m_statusServiceProvider.get(), document);
  
  NSWindowController * controller = [[self window] windowController];
  NSDocument * container = [controller document];
  
  quip::Extent cellSize = m_drawingServiceProvider->cellSize();
  CGRect frame = [self frame];
  CGRect parent = [[self superview] frame];
  CGFloat height = MAX(parent.size.height, cellSize.height() * (document->rows() + 1));
  [self setFrameSize:NSMakeSize(frame.size.width, height)];
  
  // Bind controller signals.
  m_scrollToLocationToken = m_context->controller().scrollToLocation.connect([=] (quip::Location location) {
    [self scrollToLocation:location];
  });
  
  m_scrollLocationIntoViewToken = m_context->controller().scrollLocationIntoView.connect([=] (quip::Location location) {
    [self scrollLocationIntoView:location];
  });
  
  m_documentModifiedToken = m_context->document().onDocumentModified().connect([=] () {
    CGFloat height = MAX(parent.size.height, cellSize.height() * (document->rows() + 1));
    [self setFrameSize:NSMakeSize(frame.size.width, height)];
  });
  
  m_transactionAppliedToken = m_context->onTransactionApplied().connect([=] (quip::ChangeType type) {
    switch(type) {
      case quip::ChangeType::Undo:
        [container updateChangeCount:NSChangeUndone];
        break;
      case quip::ChangeType::Redo:
        [container updateChangeCount:NSChangeRedone];
        break;
      case quip::ChangeType::Do:
      default:
        [container updateChangeCount:NSChangeDone];
        break;
    }
  });
  
  [self setNeedsDisplay:YES];
}

- (void)setStatus:(QuipStatusView *)status {
  m_statusView = status;
  m_statusServiceProvider = std::make_unique<quip::StatusServiceProvider>(status);
}

- (void)setActBackgrounded:(BOOL)shouldActBackgrounded {
  m_shouldDrawSelections = !shouldActBackgrounded;
  [self resetCursorBlink];
}

- (void)scrollLocationIntoView:(quip::Location)location {
  quip::Extent cellSize = m_drawingServiceProvider->cellSize();
  quip::Rectangle rectangle(self.frame.origin.x, self.frame.origin.y, self.frame.size.width, self.frame.size.height);
  quip::Coordinate coordinate = m_drawingServiceProvider->coordinateForLocationInFrame(location, rectangle);
  CGRect target = CGRectMake(coordinate.x, coordinate.y, cellSize.width(), cellSize.height());
  [self scrollRectToVisible:target];
}

- (QuipPopupView *)createPopupAtLocation:(NSString *)text atLocation:(quip::Location)location {
  NSMutableArray* strings = [[NSMutableArray alloc] init];
  [text enumerateLinesUsingBlock:^(NSString* line, BOOL* stop) {
    [strings addObject:line];
    *stop = NO;
  }];
  
  quip::Extent cellSize = m_drawingServiceProvider->cellSize();
  CGFloat width = cellSize.width() * [text length];
  CGFloat height = cellSize.height() * [strings count];
  CGFloat x = location.column() * cellSize.width();
  CGFloat y = self.frame.size.height - height - cellSize.height();
  
  QuipPopupView * popup = [[QuipPopupView alloc] initWithFrame:CGRectMake(x, y - 2.0, width, height)];
  [popup setContent:strings];
  [popup setDuration:0.5];
  
  [self addSubview:popup];
  return popup;
}

- (void)scrollToLocation:(quip::Location)location {
  quip::Extent cellSize = m_drawingServiceProvider->cellSize();
  CGFloat y = self.frame.size.height - (cellSize.height() * (location.row() + 1));
  
  // The parent of this view is the NSClipView, which will have the height of the
  // visible portion of the document, which can be used to bias the target point
  // to the center of the view.
  CGFloat bias = self.superview.frame.size.height * 0.5;
  [self scrollPoint:CGPointMake(0.0, y - bias)];
}

- (void)drawSelections:(const quip::SelectionDrawInfo &)drawInfo context:(CGContextRef)context {
  quip::Extent cellSize = m_drawingServiceProvider->cellSize();
  quip::Rectangle viewFrame = quip::Rectangle(self.frame.origin.x, self.frame.origin.y, self.frame.size.width, self.frame.size.height);
  quip::Document & document = m_context->document();
  for (const quip::Selection & selection : drawInfo.selections) {
    const quip::Location & lower = selection.origin();
    const quip::Location & upper = selection.extent();
    std::size_t row = lower.row();
    
    do {
      std::size_t firstColumn = row == lower.row() ? lower.column() : 0;
      std::size_t lastColumn = row == upper.row() ? upper.column() : document.row(row).size() - 1;
      
      CGFloat x = gMargin + (firstColumn * cellSize.width());
      CGFloat y = self.frame.size.height - cellSize.height() - (row * cellSize.height());
      const quip::Color & color = selection == drawInfo.selections.primary() ? drawInfo.primaryColor : drawInfo.secondaryColor;
      
      if (m_shouldDrawCursor || (drawInfo.flags & quip::CursorFlags::Blink) == 0) {
        switch (drawInfo.style) {
          case quip::CursorStyle::VerticalBlock:
            m_drawingServiceProvider->fillRectangle(quip::Rectangle(x, y - 2.0, cellSize.width() * (lastColumn + 1 - firstColumn), 0.75 * cellSize.height()), color);
            break;
          case quip::CursorStyle::VerticalBlockHalf:
            m_drawingServiceProvider->fillRectangle(quip::Rectangle(x, y - 2.0, cellSize.width() * (lastColumn + 1 - firstColumn), 0.25 * cellSize.height()), color);
            break;
          case quip::CursorStyle::VerticalBar:
            m_drawingServiceProvider->drawBarBefore(quip::Location(firstColumn, row), color, viewFrame);
            break;
          case quip::CursorStyle::Underline:
          default:
            m_drawingServiceProvider->drawUnderline(row, firstColumn, lastColumn, color, viewFrame);
            break;
        }
      }
      
      ++row;
    } while (row <= upper.row());
  }
}

- (void)drawRect:(NSRect)dirtyRect {
  CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
  
  // Clear the background.
  quip::Rectangle rectangle(dirtyRect.origin.x, dirtyRect.origin.y, dirtyRect.size.width, dirtyRect.size.height);
  m_drawingServiceProvider->fillRectangle(rectangle, quip::Color::white());
  
  if (m_context != nullptr) {
    quip::Document & document = m_context->document();

    // Find the document's extension.
    std::size_t index = m_context->document().path().find_last_of('.');
    std::string extension = "";
    const quip::FileType * fileType = nullptr;
    if (index != std::string::npos) {
      extension = m_context->document().path().substr(index + 1);
    }

    // Find the document's type.
    fileType = m_context->fileTypeDatabase().lookupByExtension(extension);
    
    // Draw selections and overlays first (text is drawn over them).
    if (m_shouldDrawSelections) {
      NSColor * systemHighlightColor = [[NSColor selectedTextBackgroundColor] colorUsingColorSpaceName:NSCalibratedRGBColorSpace];
      quip::Color primaryColor([systemHighlightColor redComponent], [systemHighlightColor greenComponent], [systemHighlightColor blueComponent]);
      quip::Color secondaryColor(primaryColor.r() * 0.5f, primaryColor.g() * 0.5f, primaryColor.b() * 0.5f);
      
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
    quip::Extent cellSize = m_drawingServiceProvider->cellSize();
    CGFloat y = self.frame.size.height - cellSize.height();
    for (std::size_t row = 0; row < document.rows(); ++row) {
      // Only draw the row if it clips into the dirty rectangle.
      CGRect rowFrame = CGRectMake(gMargin, y, self.frame.size.width - (2.0 *  - gMargin), cellSize.height());
      if (CGRectIntersectsRect(dirtyRect, rowFrame)) {
        std::vector<quip::AttributeRange> syntaxAttributes = fileType->syntax->parse(m_context->document().row(row), m_context->document().path());
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
        CGContextSetTextPosition(context, gMargin, y);
        CTLineDraw(line, context);
        
        CFRelease(line);
        CFRelease(attributed);
        CFRelease(text);
      }
      
      y -= cellSize.height();
    }
    
    quip::StatusService & status = m_context->statusService();
    status.setStatus(m_context->mode().status().c_str());
    status.setFileType(fileType->name);
    status.setLineCount(m_context->document().rows());
  }
}

@end
