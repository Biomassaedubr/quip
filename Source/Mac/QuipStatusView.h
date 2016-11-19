#import <Cocoa/Cocoa.h>

#include "DrawingService.hpp"

#include <cstddef>

@interface QuipStatusView : NSView
- (void)attachDrawingService:(quip::DrawingService *)drawingService;

- (void)setStatus:(const char *)status;
- (void)setLineCount:(std::size_t)lineCount;
- (void)setFileType:(const char *)fileType;

@end
