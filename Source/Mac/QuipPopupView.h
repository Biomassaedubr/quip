#import <Cocoa/Cocoa.h>

@interface QuipPopupView : NSView

@property (retain, atomic) NSString * content;
@property (assign, atomic) CGFloat duration;

@end
