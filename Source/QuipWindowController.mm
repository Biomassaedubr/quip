#import "QuipWindowController.h"

#import "QuipDocument.h"
#import "QuipTextView.h"
#import "QuipStatusView.h"

@interface QuipWindowController () {
@private
  NSView * m_rootView;
  QuipStatusView * m_QuipStatusView;
  NSScrollView * m_scrollView;
  QuipTextView * m_documentView;
}

@end

namespace {
  static CGFloat gQuipStatusViewHeight = 20.0;
}

@implementation QuipWindowController

- (void)windowDidLoad {
  [super windowDidLoad];
  
  CGRect rootFrame = [[self window] contentRectForFrameRect:[[self window] frame]];
  m_rootView = [[NSView alloc] initWithFrame:rootFrame];
  
  CGRect statusFrame = CGRectMake(0.0, 0.0, rootFrame.size.width, gQuipStatusViewHeight);
  m_QuipStatusView = [[QuipStatusView alloc] initWithFrame:statusFrame];
  
  CGRect contentFrame = CGRectMake(0.0, gQuipStatusViewHeight, rootFrame.size.width, rootFrame.size.height - gQuipStatusViewHeight);
  
  QuipDocument * document = [self document];
  m_documentView = [[QuipTextView alloc] initWithFrame:contentFrame document:[document document] status:m_QuipStatusView];
  m_scrollView = [[NSScrollView alloc] initWithFrame:contentFrame];
  [m_scrollView setHasHorizontalScroller:YES];
  [m_scrollView setHasVerticalScroller:YES];
  [m_scrollView setDocumentView:m_documentView];
  
  // Ensure the clip view is scrolled to the top of the document.
  [m_scrollView.contentView scrollToPoint:CGPointMake(0.0, NSMaxY(m_documentView.frame) - NSHeight(m_scrollView.contentView.bounds))];
  
  [m_rootView addSubview:m_scrollView];
  [m_rootView addSubview:m_QuipStatusView];
  
  NSWindow * window = [self window];
  [window setContentView:m_rootView];
  [window makeKeyAndOrderFront:self];
}

@end
