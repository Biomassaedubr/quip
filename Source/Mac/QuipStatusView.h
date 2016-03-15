#import <Cocoa/Cocoa.h>

#include <cstddef>

@interface QuipStatusView : NSView

- (void)setStatus:(const char *)status;
- (void)setLineCount:(std::size_t)lineCount;
- (void)setFileType:(const char *)fileType;

@end
