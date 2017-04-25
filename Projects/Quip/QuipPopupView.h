#import <Cocoa/Cocoa.h>

#include "DrawingService.hpp"

@interface QuipPopupView : NSView

@property (retain, atomic) NSMutableArray<NSString*> * content;
@property (assign, atomic) CGFloat duration;

- (void)attachDrawingService:(quip::DrawingService *)drawingService;

@end
