#import "QuipWindowController.h"

#import "QuipDocument.h"
#import "QuipTextView.h"
#import "QuipStatusView.h"

#include "DrawingServiceProvider.hpp"

@interface QuipWindowController () {
@private
  std::unique_ptr<quip::DrawingService> m_drawingService;
}
@end

@implementation QuipWindowController

- (void)windowDidLoad {
  [super windowDidLoad];

  m_drawingService = std::make_unique<quip::DrawingServiceProvider>("Menlo", 13.0f);
  
  [[self textView] attachDrawingService:m_drawingService.get()];
  [[self statusView] attachDrawingService:m_drawingService.get()];
  
  [[self textView] setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
  [[self statusView] setAutoresizingMask:NSViewWidthSizable];
  [[self scrollView] setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
  [[self scrollView] setBackgroundColor:[NSColor colorWithWhite:1.0 alpha:1.0]];
  
  QuipDocument * document = [self document];
  [[self textView] setStatus:[self statusView]];
  [[self textView] setDocument:[document document]];

  // Ensure the clip view is scrolled to the top of the document.
  [[[self scrollView] contentView] scrollToPoint:CGPointMake(0.0, NSMaxY([[self textView] frame]) - NSHeight([[[self scrollView] contentView] bounds]))];
  
  NSWindow * window = [self window];
  [window setDelegate:self];
}

- (void)windowWillClose:(NSNotification *)notification {
  [[self statusView] attachDrawingService:nullptr];
  [[self textView] attachDrawingService:nullptr];
  
  m_drawingService.reset();
}

- (void)setDocument:(id)document {
  [super setDocument:document];
  
  if (document != nullptr) {
    QuipDocument * container = (QuipDocument *)document;

    [[self textView] setDocument:[container document]];
    [[[self scrollView] contentView] scrollToPoint:CGPointMake(0.0, NSMaxY([[self textView] frame]) - NSHeight([[[self scrollView] contentView] bounds]))];
  }
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
  [[self textView] setActBackgrounded:NO];
}

- (void)windowDidResignKey:(NSNotification *)notification {
  [[self textView] setActBackgrounded:YES];
}

@end
