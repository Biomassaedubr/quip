#import "QuipPopupView.h"

#include "DrawingServiceProvider.hpp"

@interface QuipPopupView () {
@private
  std::unique_ptr<quip::DrawingService> m_drawingServiceProvider;
}
@end

@implementation QuipPopupView

- (instancetype)initWithFrame:(NSRect)frameRect {
  self = [super initWithFrame:frameRect];
  if (self != nil) {
    m_drawingServiceProvider = std::make_unique<quip::DrawingServiceProvider>("Menlo", 13.0f);
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
    std::string string = [row cStringUsingEncoding:NSUTF8StringEncoding];
    m_drawingServiceProvider->drawText(string, quip::Coordinate(0.0f, y));
    
    y -= cellSize.height();
  }
}

@end
