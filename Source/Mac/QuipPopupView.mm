#import "QuipPopupView.h"

#include "DrawingServiceProvider.hpp"

@interface QuipPopupView () {
@private
  CTFontRef m_font;
  CFDictionaryRef m_fontAttributes;
  
  std::unique_ptr<quip::DrawingServiceProvider> m_drawingServiceProvider;
}
@end

static NSString * gSizeQueryString = @"m";

@implementation QuipPopupView

- (instancetype)initWithFrame:(NSRect)frameRect {
  self = [super initWithFrame:frameRect];
  if (self != nil) {
    m_drawingServiceProvider = std::make_unique<quip::DrawingServiceProvider>("Menlo", 13.0f);

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
  
  quip::Extent cellSize = m_drawingServiceProvider->cellSize();
  CGFloat y = self.frame.size.height - cellSize.height();
  for (NSString * row in [self content]) {
    CFStringRef string = (__bridge CFStringRef)row;
    CFMutableAttributedStringRef attributed = CFAttributedStringCreateMutable(kCFAllocatorDefault, CFStringGetLength(string));
    
    CFAttributedStringBeginEditing(attributed);
    CFAttributedStringReplaceString(attributed, CFRangeMake(0, 0), string);
    CFAttributedStringSetAttributes(attributed, CFRangeMake(0, CFStringGetLength(string)), m_fontAttributes, YES);
    CFAttributedStringEndEditing(attributed);
    
    CTLineRef line = CTLineCreateWithAttributedString(attributed);
    CGContextSetTextPosition(context, 0.0f, y);
    CTLineDraw(line, context);
    
    CFRelease(line);
    CFRelease(attributed);
    
    y -= cellSize.height();
  }
}

@end
