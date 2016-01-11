#import "StatusView.h"

@interface StatusView () {
@private
  CTFontRef m_font;
  CFDictionaryRef m_fontAttributes;
  
  CGSize m_cellSize;
  NSString * m_text;
}
@end

static NSString * gSizeQueryString = @"m";

static CGFloat gStatusLineBottomPadding = 4.0;
static CGFloat gStatusLineLeftPadding = 2.0;

@implementation StatusView

- (instancetype)initWithFrame:(NSRect)frame {
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
    
    m_text = @"";
  }
  
  return self;
}

- (void)dealloc {
  CFRelease(m_fontAttributes);
  CFRelease(m_font);
}

- (void)setStatus:(const char *)status {
  m_text = [NSString stringWithCString:status encoding:NSUTF8StringEncoding];
  [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect {
  [super drawRect:dirtyRect];
  
  CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
  CFStringRef text = (__bridge CFStringRef)m_text;
  CFAttributedStringRef attributed = CFAttributedStringCreate(kCFAllocatorDefault, text, m_fontAttributes);
  CTLineRef line = CTLineCreateWithAttributedString(attributed);
  
  CGContextSetTextPosition(context, gStatusLineLeftPadding, gStatusLineBottomPadding);
  CTLineDraw(line, context);
  
  CFRelease(line);
  CFRelease(attributed);
}

@end
