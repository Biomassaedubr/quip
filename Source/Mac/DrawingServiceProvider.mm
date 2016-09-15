#import "DrawingServiceProvider.hpp"

namespace quip {
  DrawingServiceProvider::DrawingServiceProvider(const std::string & fontName, float fontSize) {
    NSString * name = [NSString stringWithUTF8String:fontName.c_str()];
    NSDictionary * attributes = @{NSFontAttributeName: [NSFont fontWithName:name size:fontSize]};
    CGSize size = [@"m" sizeWithAttributes:attributes];
    setCellSize(Extent(size.width, size.height));
  }
  
  DrawingServiceProvider::~DrawingServiceProvider() {
  }
}