#import "ApplicationDelegate.h"

#import "QuipView.h"

#include "Document.hpp"

#include <memory>

@interface ApplicationDelegate () {
@private
  NSWindow * m_window;
}
@end

@implementation ApplicationDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
  CGRect frame = CGRectMake(0.0f, 0.0f, 1024.0f, 768.0f);
  
  std::shared_ptr<quip::Document> document = std::make_shared<quip::Document>("Welcome to Quip!\nQuip is a modal text editor.");
  QuipView * view = [[QuipView alloc] initWithFrame:frame document:document];
  
  m_window = [[NSWindow alloc] initWithContentRect:frame styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:YES];
  [m_window setTitle:@"Untitled"];
  [m_window center];
  [m_window setContentView:view];
  
  [m_window makeKeyAndOrderFront:self];
}

@end
