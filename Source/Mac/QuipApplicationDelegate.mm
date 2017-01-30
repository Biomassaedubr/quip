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
    m_settings = std::make_unique<quip::GlobalSettings>();

    NSBundle* mainBundle = [NSBundle mainBundle];
    NSString* scriptRootPath = [[mainBundle resourcePath] stringByAppendingPathComponent:@"Runtime"];
    m_scriptHost = std::make_unique<quip::ScriptHost>([scriptRootPath cStringUsingEncoding:NSUTF8StringEncoding], *m_settings);
    m_scriptHost->addNativePackagePath([[mainBundle resourcePath] cStringUsingEncoding:NSUTF8StringEncoding]);
    
    // Run the boot script.
    quip::Script bootScript = m_scriptHost->getScript(m_scriptHost->scriptRootPath() + "/boot.lua");
    m_scriptHost->runScript(bootScript);
  }
  
  return self;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
  m_settings.reset();
  m_scriptHost.reset();
}

- (const quip::GlobalSettings &)settings {
  return *m_settings;
}

- (quip::ScriptHost*)scriptHost {
  return m_scriptHost.get();
}


+ (QuipApplicationDelegate *)sharedDelegate {
  QuipApplicationDelegate * delegate = [[NSApplication sharedApplication] delegate];
  return delegate;
}

@end
