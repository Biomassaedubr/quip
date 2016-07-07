#import <Cocoa/Cocoa.h>

#import "QuipStatusView.h"
#import "QuipTextView.h"

@interface QuipWindowController : NSWindowController<NSWindowDelegate>

@property (nonatomic, weak) IBOutlet NSScrollView * scrollView;
@property (nonatomic, weak) IBOutlet QuipStatusView * statusView;
@property (nonatomic, weak) IBOutlet QuipTextView * textView;

@end
