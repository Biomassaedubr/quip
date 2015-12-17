#import "QuipView.h"

#include "EditContext.hpp"
#include "Key.hpp"
#include "KeyStroke.hpp"
#include "Mode.hpp"
#include "Selection.hpp"

@interface QuipView () {
@private
  CTFontRef m_font;
  CGSize m_cellSize;
  
  std::shared_ptr<quip::EditContext> m_context;
}
@end

static NSString * gSizeQueryString = @"m";

@implementation QuipView

- (instancetype)initWithFrame:(NSRect)frame document:(std::shared_ptr<quip::Document>)document {
  self = [super initWithFrame:frame];
  if (self != nil) {
    m_font = CTFontCreateWithName(CFSTR("Menlo"), 13.0, nil);
    
    NSDictionary * attributes = @{NSFontAttributeName: [NSFont fontWithName:@"Menlo" size:13.0f]};
    m_cellSize = [gSizeQueryString sizeWithAttributes:attributes];
    
    m_context = std::make_shared<quip::EditContext>(document);
  }
  
  return self;
}

- (void)dealloc {
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

- (void)drawSelection:(quip::Selection &)selection context:(CGContextRef)context {
  quip::Document & document = m_context->document();
  const quip::Location & lower = selection.lowerBound();
  const quip::Location & upper = selection.upperBound();
  std::size_t row = lower.row();
  
  do {
    std::size_t firstColumn = row == lower.row() ? lower.column() : 0;
    std::size_t lastColumn = row == upper.row() ? upper.column() : document.row(row).size() - 1;
    
    CGFloat x = firstColumn * m_cellSize.width;
    CGFloat y = self.frame.size.height - m_cellSize.height - (row * m_cellSize.height);

    CGContextFillRect(context, CGRectMake(x, y - 2.0, m_cellSize.width * (lastColumn + 1 - firstColumn), 1));
    
    ++row;
  } while (row <= upper.row());
}

- (void)drawRect:(NSRect)dirtyRect {
  [super drawRect:dirtyRect];

  quip::Document & document = m_context->document();
  CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
  
  CFStringRef keys[] = { kCTFontAttributeName };
  CFTypeRef values[] = { m_font };
  const void ** opaqueKeys = reinterpret_cast<const void **>(&keys);
  const void ** opaqueValues = reinterpret_cast<const void **>(&values);
  CFDictionaryRef attributes = CFDictionaryCreate(kCFAllocatorDefault, opaqueKeys, opaqueValues, 1, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  
  CGFloat y = self.frame.size.height - m_cellSize.height;
  for (std::size_t row = 0; row < document.rows(); ++row) {
    CFStringRef text = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, document.row(row).c_str(), kCFStringEncodingUTF8, kCFAllocatorNull);
    CFAttributedStringRef attributed = CFAttributedStringCreate(kCFAllocatorDefault, text, attributes);
    CTLineRef line = CTLineCreateWithAttributedString(attributed);
    
    CGContextSetTextPosition(context, 0.0f, y);
    CTLineDraw(line, context);
    
    y -= m_cellSize.height;
    
    for (quip::Selection & selection : m_context->selections()) {
      [self drawSelection:selection context:context];
    }
    
    CFRelease(line);
    CFRelease(attributed);
    CFRelease(text);
  }
  
  CFRelease(attributes);
}


@end
