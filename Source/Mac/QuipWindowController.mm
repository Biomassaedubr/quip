#import "QuipWindowController.h"

#import "QuipDocument.h"
#import "QuipTextView.h"
#import "QuipStatusView.h"

@implementation QuipWindowController

- (void)windowDidLoad {
  [super windowDidLoad];

  [[self textView] setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
  [[self statusView] setAutoresizingMask:NSViewWidthSizable];
  [[self scrollView] setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
  
  QuipDocument * document = [self document];
  [[self textView] setDocument:[document document]];
  [[self textView] setStatus:[self statusView]];

  // Ensure the clip view is scrolled to the top of the document.
  [[[self scrollView] contentView] scrollToPoint:CGPointMake(0.0, NSMaxY([[self textView] frame]) - NSHeight([[[self scrollView] contentView] bounds]))];
  
  NSWindow * window = [self window];
  [window makeKeyAndOrderFront:self];
}

- (void)setDocument:(id)document {
  [super setDocument:document];
  
  if (document != nullptr) {
    QuipDocument * container = (QuipDocument *)document;

    [[self textView] setDocument:[container document]];
    [[[self scrollView] contentView] scrollToPoint:CGPointMake(0.0, NSMaxY([[self textView] frame]) - NSHeight([[[self scrollView] contentView] bounds]))];
  }
}

@end
