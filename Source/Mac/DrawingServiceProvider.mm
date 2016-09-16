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
  }
  
  DrawingServiceProvider::~DrawingServiceProvider() {
  }
  
  void DrawingServiceProvider::fillRectangle (const Rectangle & rectangle, const Color & color) {
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    CGContextSetRGBFillColor(context, color.r(), color.g(), color.b(), color.a());
    CGContextFillRect(context, makeCGRect(rectangle));
  }
}