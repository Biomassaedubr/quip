#import "ApplicationDelegate.h"

#import "QuipView.h"
#import "StatusView.h"

#include "Document.hpp"

#include <memory>

@interface ApplicationDelegate () {
@private
  NSWindow * m_window;
  
  NSView * m_rootView;
  StatusView * m_statusView;
  NSScrollView * m_scrollView;
  QuipView * m_documentView;
}
@end

namespace {
  static CGFloat gStatusViewHeight = 20.0;
}

@implementation ApplicationDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
  CGRect rootFrame = CGRectMake(0.0, 0.0, 1024.0, 768.0);
  m_rootView = [[NSView alloc] initWithFrame:rootFrame];
  
  CGRect statusFrame = CGRectMake(0.0, 0.0, rootFrame.size.width, gStatusViewHeight);
  m_statusView = [[StatusView alloc] initWithFrame:statusFrame];
  
  CGRect contentFrame = CGRectMake(0.0, gStatusViewHeight, rootFrame.size.width, rootFrame.size.height - gStatusViewHeight);
  std::shared_ptr<quip::Document> document = std::make_shared<quip::Document>("Welcome to Quip!\nQuip is a modal text editor.");
  m_documentView = [[QuipView alloc] initWithFrame:contentFrame document:document status:m_statusView];
  m_scrollView = [[NSScrollView alloc] initWithFrame:contentFrame];
  [m_scrollView setDocumentView:m_documentView];
  
  [m_rootView addSubview:m_scrollView];
  [m_rootView addSubview:m_statusView];
  
  m_window = [[NSWindow alloc] initWithContentRect:rootFrame styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:YES];
  [m_window setTitle:@"Untitled"];
  [m_window center];
  [m_window setContentView:m_rootView];
  
  [m_window makeKeyAndOrderFront:self];
}

- (void)loadDocumentFrom:(NSURL *)url {
  NSError * error;
  NSString * content = [[NSString alloc] initWithContentsOfURL:url encoding:NSUTF8StringEncoding error:&error];
  if (content == nil) {
    NSLog(@"Error opening file (%@): %@", url, [error localizedFailureReason]);
  } else {
    std::shared_ptr<quip::Document> document = std::make_shared<quip::Document>([content cStringUsingEncoding:NSUTF8StringEncoding]);
    [m_documentView setDocument:document];
    [m_window setTitle:[url path]];
  }
}

- (void)saveDocumentTo:(NSURL *)url {
  NSString * contents = [NSString stringWithUTF8String:[m_documentView document].contents().c_str()];
  NSError * error;
  [contents writeToURL:url atomically:YES encoding:NSUTF8StringEncoding error:&error];
  
  if (error != nil) {
    NSLog(@"Error saving file (%@): %@", url, [error localizedFailureReason]);
  } else {
    [m_window setTitle:[url path]];
    [m_documentView document].setPath([[url path] cStringUsingEncoding:NSUTF8StringEncoding]);
  }
}

- (IBAction)openDocument:(id)sender {
  NSOpenPanel * panel = [NSOpenPanel openPanel];
  [panel setCanChooseFiles:YES];
  [panel setCanChooseDirectories:NO];
  
  if([panel runModal] == NSFileHandlingPanelOKButton) {
    [self loadDocumentFrom:[panel URL]];
  }
}

- (IBAction)saveDocument:(id)sender {
  if ([m_documentView document].path() == "") {
    NSSavePanel * panel = [NSSavePanel savePanel];
    if ([panel runModal] == NSFileHandlingPanelOKButton) {
      [self saveDocumentTo:[panel URL]];
    }
  } else {
    NSString * path = [NSString stringWithCString:[m_documentView document].path().c_str() encoding:NSUTF8StringEncoding];
    [self saveDocumentTo:[NSURL fileURLWithPath:path]];
  }
}

- (IBAction)saveDocumentAs:(id)sender {
  NSSavePanel * panel = [NSSavePanel savePanel];
  if ([panel runModal] == NSFileHandlingPanelOKButton) {
    [self saveDocumentTo:[panel URL]];
  }
}

@end
