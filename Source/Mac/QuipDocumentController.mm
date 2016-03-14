#import "QuipDocumentController.h"

#import <objc/runtime.h>

@implementation QuipDocumentController

static const char * kOpenPanelKey = "AssociatedOpenPanel";

- (void)beginOpenPanel:(NSOpenPanel *)openPanel forTypes:(NSArray<NSString *> *)types completionHandler:(void (^)(NSInteger))completionHandler {
  NSButton * button = [[NSButton alloc] init];
  [button setTitle:@"Show Hidden Files"];
  [button setButtonType:NSSwitchButton];
  [button sizeToFit];
  
  [button setTarget:self];
  [button setAction:@selector(handleShowHiddenClicked:)];
  objc_setAssociatedObject(button, kOpenPanelKey, openPanel, OBJC_ASSOCIATION_RETAIN);
  
  [openPanel setAccessoryView:button];
  [super beginOpenPanel:openPanel forTypes:types completionHandler:completionHandler];
}

- (void)handleShowHiddenClicked:(id)sender {
  NSOpenPanel * openPanel = objc_getAssociatedObject(sender, kOpenPanelKey);
  if (openPanel != nil) {
    [openPanel setShowsHiddenFiles:[sender intValue]];
  }
}

@end
