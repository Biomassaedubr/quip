#import "QuipTextView.h"

#import "QuipStatusView.h"

#include "EditContext.hpp"
#include "Key.hpp"
#include "KeyStroke.hpp"
#include "Mode.hpp"
#include "Selection.hpp"

#include <cmath>
#include <vector>

@interface QuipTextView () {
@private
  CTFontRef m_font;
  CFDictionaryRef m_fontAttributes;
  CTParagraphStyleRef m_paragraphAttributes;
  std::vector<CTTextTabRef> m_tabStops;
  
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

static CGFloat gPrimarySelectionColor[] = { 1.0, 0.0, 0.0, 1.0 };
static CGFloat gAuxilliarySelectionColor[] = { 0.7, 0.2, 0.2, 1.0 };

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
  
  m_context->selections().replace(quip::Selection(target, target));
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

- (quip::Document &)document {
  return m_context->document();
}

- (void)setDocument:(std::shared_ptr<quip::Document>)document {
  m_context = std::make_shared<quip::EditContext>(document);
  
  CGFloat frameWidth = m_minimumFrame.size.width;
  CGFloat frameHeight = std::max(m_minimumFrame.size.height, m_cellSize.height * document->rows());
  [self setFrameSize:CGSizeMake(frameWidth, frameHeight)];
  [self setNeedsDisplay:YES];
}

- (void)setStatus:(QuipStatusView *)status {
  m_statusView = status;
}

- (void)drawSelection:(quip::Selection &)selection asPrimary:(BOOL)asPrimary context:(CGContextRef)context {
  quip::Document & document = m_context->document();
  const quip::Location & lower = selection.lowerBound();
  const quip::Location & upper = selection.upperBound();
  std::size_t row = lower.row();
  
  do {
    std::size_t firstColumn = row == lower.row() ? lower.column() : 0;
    std::size_t lastColumn = row == upper.row() ? upper.column() : document.row(row).size() - 1;
    
    CGFloat x = firstColumn * m_cellSize.width;
    CGFloat y = self.frame.size.height - m_cellSize.height - (row * m_cellSize.height);
    CGFloat * color = asPrimary ? gPrimarySelectionColor : gAuxilliarySelectionColor;
    CGContextSetRGBStrokeColor(context, color[0], color[1], color[2], color[3]);
    
    if (m_isCursorVisible) {
      switch (m_context->mode().cursorStyle()) {
        case quip::CursorStyle::VerticalBar:
          CGContextMoveToPoint(context, x, y - 2.0);
          CGContextAddLineToPoint(context, x, y + m_cellSize.height - 6.0f);
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

- (void)drawRect:(NSRect)dirtyRect {
  quip::Document & document = m_context->document();
  CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
  
  CGFloat y = self.frame.size.height - m_cellSize.height;
  for (std::size_t row = 0; row < document.rows(); ++row) {
    // Only draw the row if it clips into the dirty rectangle.
    if (CGRectIntersectsRect(dirtyRect, CGRectMake(0.0, y, self.frame.size.width, m_cellSize.height))) {
      CFStringRef text = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, document.row(row).c_str(), kCFStringEncodingUTF8, kCFAllocatorNull);
      CFAttributedStringRef attributed = CFAttributedStringCreate(kCFAllocatorDefault, text, m_fontAttributes);
      CTLineRef line = CTLineCreateWithAttributedString(attributed);
      
      CGContextSetTextPosition(context, 0.0f, y);
      CTLineDraw(line, context);
      
      CFRelease(line);
      CFRelease(attributed);
      CFRelease(text);
    }
    
    y -= m_cellSize.height;
  }
  
  if ([[self window] isKeyWindow]) {
    for (quip::Selection & selection : m_context->selections()) {
      BOOL primary = selection == m_context->selections().primary();
      [self drawSelection:selection asPrimary:primary context:context];
    }
  }
  
  [m_statusView setStatus:m_context->mode().status().c_str()];
}

@end
