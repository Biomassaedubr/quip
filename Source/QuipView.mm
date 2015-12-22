#import "QuipView.h"

#include "EditContext.hpp"
#include "Key.hpp"
#include "KeyStroke.hpp"
#include "Mode.hpp"
#include "Selection.hpp"

@interface QuipView () {
@private
  CTFontRef m_font;
  CFDictionaryRef m_fontAttributes;
  
  CGSize m_cellSize;
  
  std::shared_ptr<quip::EditContext> m_context;
}
@end

static NSString * gSizeQueryString = @"m";

static CGFloat gStatusLineBottomPadding = 4.0;
static CGFloat gStatusLineLeftPadding = 2.0;

static CGFloat gPrimarySelectionColor[] = { 1.0, 0.0, 0.0, 1.0 };
static CGFloat gAuxilliarySelectionColor[] = { 0.7, 0.2, 0.2, 1.0 };

@implementation QuipView

- (instancetype)initWithFrame:(NSRect)frame document:(std::shared_ptr<quip::Document>)document {
  self = [super initWithFrame:frame];
  if (self != nil) {
    m_font = CTFontCreateWithName(CFSTR("Menlo"), 13.0, nil);
    
    NSDictionary * attributes = @{NSFontAttributeName: [NSFont fontWithName:@"Menlo" size:13.0f]};
    m_cellSize = [gSizeQueryString sizeWithAttributes:attributes];
    
    CFStringRef keys[] = { kCTFontAttributeName };
    CFTypeRef values[] = { m_font };
    const void ** opaqueKeys = reinterpret_cast<const void **>(&keys);
    const void ** opaqueValues = reinterpret_cast<const void **>(&values);
    m_fontAttributes = CFDictionaryCreate(kCFAllocatorDefault, opaqueKeys, opaqueValues, 1, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    [self setDocument:document];
  }
  
  return self;
}

- (void)dealloc {
  CFRelease(m_fontAttributes);
  CFRelease(m_font);
}

- (BOOL)acceptsFirstResponder {
  return YES;
}

- (void)keyDown:(NSEvent *)event {
  quip::KeyStroke keyStroke(static_cast<quip::Key>(event.keyCode), std::string([[event characters] cStringUsingEncoding:NSUTF8StringEncoding]));
  if (m_context->processKey(keyStroke)) {
    [self setNeedsDisplay:YES];
  }
}

- (quip::Document &)document {
  return m_context->document();
}
- (void)setDocument:(std::shared_ptr<quip::Document>)document {
  m_context = std::make_shared<quip::EditContext>(document);
  [self setNeedsDisplay:YES];
}

- (void)drawStatusLine:(const std::string &)status context:(CGContextRef)context {
  CFStringRef text = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, status.c_str(), kCFStringEncodingUTF8, kCFAllocatorNull);
  CFAttributedStringRef attributed = CFAttributedStringCreate(kCFAllocatorDefault, text, m_fontAttributes);
  CTLineRef line = CTLineCreateWithAttributedString(attributed);
  
  CGContextSetTextPosition(context, gStatusLineLeftPadding, gStatusLineBottomPadding);
  CTLineDraw(line, context);
  
  CFRelease(line);
  CFRelease(attributed);
  CFRelease(text);
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

    CGContextMoveToPoint(context, x, y - 2.0);
    CGContextAddLineToPoint(context, x + (m_cellSize.width * (lastColumn + 1 - firstColumn)), y - 2.0);
    
    CGFloat * color = asPrimary ? gPrimarySelectionColor : gAuxilliarySelectionColor;
    CGContextSetRGBStrokeColor(context, color[0], color[1], color[2], color[3]);
    
    CGContextStrokePath(context);
    
    ++row;
  } while (row <= upper.row());
}

- (void)drawRect:(NSRect)dirtyRect {
  [super drawRect:dirtyRect];

  quip::Document & document = m_context->document();
  CGContextRef context = [[NSGraphicsContext currentContext] CGContext];

  CGFloat y = self.frame.size.height - m_cellSize.height;
  for (std::size_t row = 0; row < document.rows(); ++row) {
    CFStringRef text = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, document.row(row).c_str(), kCFStringEncodingUTF8, kCFAllocatorNull);
    CFAttributedStringRef attributed = CFAttributedStringCreate(kCFAllocatorDefault, text, m_fontAttributes);
    CTLineRef line = CTLineCreateWithAttributedString(attributed);
    
    CGContextSetTextPosition(context, 0.0f, y);
    CTLineDraw(line, context);
    
    y -= m_cellSize.height;
    
    for (quip::Selection & selection : m_context->selections()) {
      [self drawSelection:selection asPrimary:NO context:context];
    }
    
    // Re-draw the primary selection more visibly.
    [self drawSelection:m_context->selections().primary() asPrimary:YES context:context];
    
    CFRelease(line);
    CFRelease(attributed);
    CFRelease(text);
  }
  
  [self drawStatusLine:m_context->mode().status() context:context];
}


@end
