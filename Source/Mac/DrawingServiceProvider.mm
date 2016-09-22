#import "DrawingServiceProvider.hpp"

namespace quip {
  namespace {
    CGRect makeCGRect(const Rectangle & rectangle) {
      return CGRectMake(rectangle.x(), rectangle.y(), rectangle.width(), rectangle.height());
    }
  }
  
  DrawingServiceProvider::DrawingServiceProvider(const std::string & fontName, float fontSize) {
    NSString * name = [NSString stringWithUTF8String:fontName.c_str()];
    NSDictionary * attributes = @{NSFontAttributeName: [NSFont fontWithName:name size:fontSize]};
    CGSize size = [@"m" sizeWithAttributes:attributes];
    setCellSize(Extent(size.width, size.height));
    
    CFStringRef fontNameRef = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, fontName.c_str(), kCFStringEncodingUTF8, kCFAllocatorNull);
    m_font = CTFontCreateWithName(fontNameRef, fontSize, nil);
    CFRelease(fontNameRef);
    
    CFStringRef keys[] = { kCTFontAttributeName };
    CFTypeRef values[] = { m_font };
    const void ** opaqueKeys = reinterpret_cast<const void **>(&keys);
    const void ** opaqueValues = reinterpret_cast<const void **>(&values);
    m_fontAttributes = CFDictionaryCreate(kCFAllocatorDefault, opaqueKeys, opaqueValues, 1, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  }
  
  DrawingServiceProvider::~DrawingServiceProvider() {
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
  
  void DrawingServiceProvider::drawText(const std::string & text, const quip::Coordinate & coordinate) {
    CFStringRef string = CFStringCreateWithCStringNoCopy(kCFAllocatorDefault, text.c_str(), kCFStringEncodingUTF8, kCFAllocatorNull);
    CFAttributedStringRef attributed = CFAttributedStringCreate(kCFAllocatorDefault, string, m_fontAttributes);
    CTLineRef line = CTLineCreateWithAttributedString(attributed);
    
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextSetTextPosition(context, coordinate.x, coordinate.y);
    CTLineDraw(line, context);
    
    CFRelease(string);
    CFRelease(line);
    CFRelease(attributed);
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