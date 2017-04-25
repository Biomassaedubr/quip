#import "QuipPopupView.h"

#include "DrawingServiceProvider.hpp"

@interface QuipPopupView () {
@private
  quip::DrawingService * m_drawingService;
}
@end

@implementation QuipPopupView

- (instancetype)initWithFrame:(NSRect)frameRect {
  self = [super initWithFrame:frameRect];
  if (self != nil) {
    m_drawingService = nullptr;
  }
  
  return self;
}

- (void)attachDrawingService:(quip::DrawingService *)drawingService {
  m_drawingService = drawingService;
}

- (void)drawRect:(NSRect)dirtyRect {
  CGContextRef context = [[NSGraphicsContext currentContext] CGContext];

  CGContextSetRGBFillColor(context, 1.0, 1.0, 0.0, 0.2);
  CGContextFillRect(context, dirtyRect);
  
  quip::Extent cellSize = m_drawingService->cellSize();
  CGFloat y = self.frame.size.height - cellSize.height();
  for (NSString * row in [self content]) {
    std::string string = [row cStringUsingEncoding:NSUTF8StringEncoding];
    m_drawingService->drawText(string, quip::Coordinate(0.0f, y));
    
    y -= cellSize.height();
  }
}

@end
