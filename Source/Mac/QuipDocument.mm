#import "QuipDocument.h"

#import "QuipWindowController.h"

@interface QuipDocument () {
@private
  std::shared_ptr<quip::Document> m_document;
}

@end

@implementation QuipDocument

- (instancetype)init {
  self = [super init];
  if (self != nil) {
    m_document = std::make_shared<quip::Document>();
  }
  
  return self;
}

- (std::shared_ptr<quip::Document>)document {
  return m_document;
}

- (void)makeWindowControllers {
  QuipWindowController * controller = [[QuipWindowController alloc] initWithWindowNibName:@"QuipWindowController"];
  [self addWindowController:controller];
}

- (NSData *)dataOfType:(NSString *)type error:(NSError **)error {
  std::string contents = m_document->contents().c_str();
  NSData * data = [NSData dataWithBytes:contents.c_str() length:contents.length()];

  return data;
}

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)type error:(NSError **)error {
  const char * start = reinterpret_cast<const char *>([data bytes]);
  const char * end = start + [data length];
  std::string content(start, end);
  
  m_document = std::make_shared<quip::Document>(content);
  NSString * path = [[self fileURL] path];
  m_document->setPath([path cStringUsingEncoding:NSUTF8StringEncoding]);

  return YES;
}

- (void)saveToURL:(NSURL*)url ofType:(NSString*)typeName forSaveOperation:(NSSaveOperationType)saveOperation completionHandler:(void (^)(NSError* _Nullable))completionHandler {
  // The intermediate directories of a proxy document may not exist, so create them if needed.
  // If this fails, the super version will also fail and display an appropriate error sheet.
  NSFileManager* manager = [NSFileManager defaultManager];
  NSString* folder = [[url path] stringByDeletingLastPathComponent];
  if (![manager fileExistsAtPath:folder]) {
    [manager createDirectoryAtPath:folder withIntermediateDirectories:YES attributes:nil error:nil];
  }

  [super saveToURL:url ofType:typeName forSaveOperation:saveOperation completionHandler:completionHandler];
}

+ (BOOL)autosavesInPlace {
  return NO;
}

@end
