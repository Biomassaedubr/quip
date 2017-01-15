#import "QuipApplicationDelegate.h"

@interface QuipApplicationDelegate () {
@private
  std::unique_ptr<quip::GlobalSettings> m_settings;
}

@end

@implementation QuipApplicationDelegate

- (instancetype)init {
  self = [super init];
  if (self != nil) {
    m_settings = std::make_unique<quip::GlobalSettings>();
  }
  
  return self;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
  m_settings.reset();
}

- (const quip::GlobalSettings &)settings {
  return *m_settings;
}

+ (QuipApplicationDelegate *)sharedDelegate {
  QuipApplicationDelegate * delegate = [[NSApplication sharedApplication] delegate];
  return delegate;
}

@end
