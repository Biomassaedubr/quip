#import "QuipWindowController.h"

#import "QuipDocument.h"
#import "QuipTextView.h"
#import "QuipStatusView.h"

@implementation QuipWindowController

- (void)windowDidLoad {
  [super windowDidLoad];
  
  QuipDocument * document = [self document];
  
  [[self textView] setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
  [[self textView] setDocument:[document document]];
  [[self textView] setStatus:[self statusView]];

  [[self statusView] setAutoresizingMask:NSViewWidthSizable];

  [[self scrollView] setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
  
  // Ensure the clip view is scrolled to the top of the document.
  [[[self scrollView] contentView] scrollToPoint:CGPointMake(0.0, NSMaxY([[self textView] frame]) - NSHeight([[[self scrollView] contentView] bounds]))];
  
  NSWindow * window = [self window];
  [window makeKeyAndOrderFront:self];
}

@end
