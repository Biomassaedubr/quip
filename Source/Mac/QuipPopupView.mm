#import "QuipPopupView.h"

@interface QuipPopupView () {
@private
  CTFontRef m_font;
  CFDictionaryRef m_fontAttributes;
}
@end

@implementation QuipPopupView

- (instancetype)initWithFrame:(NSRect)frameRect {
  self = [super initWithFrame:frameRect];
  if (self != nil) {
    m_font = CTFontCreateWithName(CFSTR("Menlo"), 13.0, nil);
    
    CFStringRef keys[] = { kCTFontAttributeName };
    CFTypeRef values[] = { m_font };
    const void ** opaqueKeys = reinterpret_cast<const void **>(&keys);
    const void ** opaqueValues = reinterpret_cast<const void **>(&values);
    m_fontAttributes = CFDictionaryCreate(kCFAllocatorDefault, opaqueKeys, opaqueValues, 1, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
  }
  
  return self;
}

- (void)drawRect:(NSRect)dirtyRect {
  CGContextRef context = [[NSGraphicsContext currentContext] CGContext];

  CGContextSetRGBFillColor(context, 1.0, 1.0, 0.0, 0.2);
  CGContextFillRect(context, dirtyRect);
  
  CFStringRef text = (__bridge CFStringRef)[self content];
  CFMutableAttributedStringRef attributed = CFAttributedStringCreateMutable(kCFAllocatorDefault, CFStringGetLength(text));
  
  CFAttributedStringBeginEditing(attributed);
  CFAttributedStringReplaceString(attributed, CFRangeMake(0, 0), text);
  CFAttributedStringSetAttributes(attributed, CFRangeMake(0, CFStringGetLength(text)), m_fontAttributes, YES);
  CFAttributedStringEndEditing(attributed);
  
  CTLineRef line = CTLineCreateWithAttributedString(attributed);
  CGContextSetTextPosition(context, 0.0f, 5.0f);
  CTLineDraw(line, context);
  
  CFRelease(line);
  CFRelease(attributed);
}

@end
