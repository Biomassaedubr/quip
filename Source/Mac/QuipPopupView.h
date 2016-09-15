#import <Cocoa/Cocoa.h>

@interface QuipPopupView : NSView

@property (retain, atomic) NSMutableArray<NSString*> * content;
@property (assign, atomic) CGFloat duration;

@end
