#import "QuipApplicationDelegate.h"

#include "Script.hpp"
#include "ScriptHost.hpp"

@interface QuipApplicationDelegate () {
@private
  std::unique_ptr<quip::ScriptHost> m_scriptHost;
  std::unique_ptr<quip::GlobalSettings> m_settings;
}

@end

@implementation QuipApplicationDelegate

- (instancetype)init {
  self = [super init];
  if (self != nil) {
    NSBundle* mainBundle = [NSBundle mainBundle];
    m_scriptHost = std::make_unique<quip::ScriptHost>();
    m_scriptHost->addNativePackagePath([[mainBundle resourcePath] cStringUsingEncoding:NSUTF8StringEncoding]);
    m_settings = std::make_unique<quip::GlobalSettings>();
    
    // Run the boot script.
    NSString* bootScriptPath = [[NSBundle mainBundle] pathForResource:@"boot" ofType:@"lua"];
    quip::Script bootScript = m_scriptHost->getScript([bootScriptPath cStringUsingEncoding:NSUTF8StringEncoding]);
    m_scriptHost->runScript(bootScript);
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
