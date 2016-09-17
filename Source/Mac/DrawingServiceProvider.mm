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
}