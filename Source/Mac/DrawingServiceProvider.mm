#import "DrawingServiceProvider.hpp"

#include "AttributeRange.hpp"
#include "GlobalSettings.hpp"

namespace quip {
  namespace {
    static void initializeHighlight (std::unordered_map<std::string, Highlight>& mapping, const std::string& name, quip::Color foreground) {
      Highlight result;
      result.foregroundColor = CGColorCreateGenericRGB(foreground.r(), foreground.g(), foreground.b(), foreground.a());
      
      CFStringRef keys[] = { kCTForegroundColorAttributeName };
      CFTypeRef values[] = { result.foregroundColor };
      const void ** opaqueKeys = reinterpret_cast<const void **>(&keys);
      const void ** opaqueValues = reinterpret_cast<const void **>(&values);
      result.attributes = CFDictionaryCreate(kCFAllocatorDefault, opaqueKeys, opaqueValues, 1, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
      
      mapping[name] = result;
    }
    
    static void releaseHighlight (Highlight * highlight) {
      CFRelease(highlight->attributes);
      CFRelease(highlight->foregroundColor);
    }
    
    CGRect makeCGRect(const Rectangle & rectangle) {
      return CGRectMake(rectangle.x(), rectangle.y(), rectangle.width(), rectangle.height());
    }
  }
  
  DrawingServiceProvider::DrawingServiceProvider(const GlobalSettings & settings) {
    NSString * name = [NSString stringWithUTF8String:settings.fontFace().c_str()];
    NSDictionary * attributes = @{NSFontAttributeName: [NSFont fontWithName:name size:settings.fontSize()]};
    CGSize size = [@"m" sizeWithAttributes:attributes];
    setCellSize(Extent(size.width, size.height));
    
    CFStringRef fontNameRef = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, settings.fontFace().c_str(), kCFStringEncodingUTF8, kCFAllocatorNull);
    m_font = CTFontCreateWithName(fontNameRef, settings.fontSize(), nil);
    CFRelease(fontNameRef);
    
    CFStringRef keys[] = { kCTFontAttributeName, kCTParagraphStyleAttributeName };
    CFTypeRef values[] = { m_font };
    const void ** opaqueKeys = reinterpret_cast<const void **>(&keys);
    const void ** opaqueValues = reinterpret_cast<const void **>(&values);
    m_fontAttributes = CFDictionaryCreate(kCFAllocatorDefault, opaqueKeys, opaqueValues, 1, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    initializeHighlight(m_highlightAttributes, "Keyword", quip::Color(0.0f, 0.0f, 1.0f));
    initializeHighlight(m_highlightAttributes, "Preprocessor", quip::Color(0.5f, 0.25f, 0.1f));
    initializeHighlight(m_highlightAttributes, "Comment", quip::Color(0.0f, 0.5f, 0.0f));
  }
  
  DrawingServiceProvider::~DrawingServiceProvider() {
    auto cursor = m_highlightAttributes.begin();
    auto end = m_highlightAttributes.end();
    while (cursor != end) {
      releaseHighlight(&cursor->second);
      ++cursor;
    }
    
    CFRelease(m_fontAttributes);
    CFRelease(m_font);
  }
  
  void DrawingServiceProvider::fillRectangle (const Rectangle & rectangle, const Color & color) {
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextSetRGBFillColor(context, color.r(), color.g(), color.b(), color.a());
    CGContextFillRect(context, makeCGRect(rectangle));
  }
  
  void DrawingServiceProvider::drawUnderline (std::size_t row, std::size_t firstColumn, std::size_t lastColumn, const Color & color, const Rectangle & frame)
  {
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextSetRGBStrokeColor(context, color.r(), color.g(), color.b(), color.a());

    Coordinate origin = coordinateForLocationInFrame(Location(firstColumn, row), frame);
    Coordinate extent = coordinateForLocationInFrame(Location(lastColumn, row), frame);
    
    CGContextMoveToPoint(context, origin.x, origin.y - 2.0);
    CGContextAddLineToPoint(context, extent.x + cellSize().width(), extent.y - 2.0);
    CGContextStrokePath(context);
  }
  
  void DrawingServiceProvider::drawBarBefore (const Location & location, const Color & color, const Rectangle & frame)
  {
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextSetRGBStrokeColor(context, color.r(), color.g(), color.b(), color.a());
    Coordinate origin = coordinateForLocationInFrame(location, frame);
    
    CGContextMoveToPoint(context, origin.x, origin.y - 2.0);
    CGContextAddLineToPoint(context, origin.x, origin.y + cellSize().height() - 6.0);
    CGContextStrokePath(context);
  }

  void DrawingServiceProvider::drawBarAfter (const Location & location, const Color & color, const Rectangle & frame)
  {
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextSetRGBStrokeColor(context, color.r(), color.g(), color.b(), color.a());
    Coordinate origin = coordinateForLocationInFrame(location, frame);
    
    CGContextMoveToPoint(context, origin.x + cellSize().width(), origin.y - 2.0);
    CGContextAddLineToPoint(context, origin.x + cellSize().width(), origin.y + cellSize().height() - 6.0);
    CGContextStrokePath(context);
  }

  void DrawingServiceProvider::drawText(const std::string & text, const quip::Coordinate & coordinate, const std::vector<AttributeRange> & attributes) {
    CFStringRef string = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, text.c_str(), kCFStringEncodingUTF8, kCFAllocatorNull);
    CFMutableAttributedStringRef attributed = CFAttributedStringCreateMutable(kCFAllocatorDefault, CFStringGetLength(string));
    
    CFAttributedStringBeginEditing(attributed);
    CFAttributedStringReplaceString(attributed, CFRangeMake(0, 0), string);
    CFAttributedStringSetAttributes(attributed, CFRangeMake(0, CFStringGetLength(string)), m_fontAttributes, YES);
    
    for (const AttributeRange& range : attributes) {
      CFAttributedStringSetAttributes(attributed, CFRangeMake(range.start, range.length), m_highlightAttributes[range.name].attributes, NO);
    }
    
    CFAttributedStringEndEditing(attributed);
    
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CTLineRef line = CTLineCreateWithAttributedString(attributed);
    CGContextSetTextPosition(context, coordinate.x, coordinate.y);
    CTLineDraw(line, context);
    
    CFRelease(line);
    CFRelease(attributed);
    CFRelease(string);
  }
  
  Rectangle DrawingServiceProvider::measureText(const std::string & text) {
    CFStringRef string = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, text.c_str(), kCFStringEncodingUTF8, kCFAllocatorNull);
    CFAttributedStringRef attributed = CFAttributedStringCreate(kCFAllocatorDefault, string, m_fontAttributes);
    CTLineRef line = CTLineCreateWithAttributedString(attributed);
    CGRect bounds = CTLineGetBoundsWithOptions(line, 0);
    
    CFRelease(string);
    CFRelease(line);
    CFRelease(attributed);
    
    return Rectangle(bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height);
  }
}
