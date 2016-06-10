#import "QuipDocumentController.h"

#import "QuipDocument.h"

#import <objc/runtime.h>

@interface QuipDocumentController () {
@private
  QuipDocument * m_transientDocument;
}

@end

@implementation QuipDocumentController

static const char * kOpenPanelKey = "AssociatedOpenPanel";

static BOOL canDocumentBeTransient (NSDocument * document) {
  QuipDocument * container = (QuipDocument *)document;
  if (container == nil) {
    return YES;
  }
  
  std::shared_ptr<quip::Document> contained = [container document];
  if (contained == nullptr) {
    return YES;
  }
  
  return contained->isEmpty() && contained->path().size() == 0;
}

- (instancetype)init {
  self = [super init];
  if (self != nil) {
    m_transientDocument = nil;
  }
  
  return self;
}

- (void)newDocument:(id)sender {
  // Explicitly creating a new document clears the transient document, if it existed.
  m_transientDocument = nil;
  
  [super newDocument:sender];
}

- (void)addDocument:(NSDocument *)document {
  if ([[self documents] count] == 0) {
    if (canDocumentBeTransient(document)) {
      // This is the first document, and it is new, so it becomes the transient document.
      m_transientDocument = (QuipDocument *)document;
    }
  }
  
  [super addDocument:document];
}

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

- (void)openDocumentWithContentsOfURL:(NSURL *)url display:(BOOL)displayDocument completionHandler:(void (^)(NSDocument * _Nullable, BOOL, NSError * _Nullable))completionHandler {
  // Verify that the transient document can still be considered transient before replacing
  // it (it may have been edited since it was recorded).
  if (m_transientDocument != nil && canDocumentBeTransient(m_transientDocument)) {
    void (^replacementHandler)(NSDocument *, BOOL, NSError *) = ^(NSDocument * document, BOOL, NSError *) {
      [self replaceDocument:m_transientDocument withDocument:(QuipDocument *)document];
      m_transientDocument = nil;
      
      completionHandler(document, NO, nil);
    };
    
    [super openDocumentWithContentsOfURL:url display:NO completionHandler:replacementHandler];
  } else {
    [super openDocumentWithContentsOfURL:url display:displayDocument completionHandler:completionHandler];
  }
}

- (void)handleShowHiddenClicked:(id)sender {
  NSOpenPanel * openPanel = objc_getAssociatedObject(sender, kOpenPanelKey);
  if (openPanel != nil) {
    [openPanel setShowsHiddenFiles:[sender intValue]];
  }
}

- (void)replaceDocument:(QuipDocument *)existing withDocument:(QuipDocument *)replacement {
  NSArray * controllers = [[existing windowControllers] copy];
  
  for (NSWindowController * controller in [existing windowControllers]) {
    [existing removeWindowController:controller];
  }
  
  [existing close];
  
  for (NSWindowController * controller in controllers) {
    [replacement addWindowController:controller];
  }
}

@end
