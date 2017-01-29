#import <Cocoa/Cocoa.h>

#include "GlobalSettings.hpp"
#include "ScriptHost.hpp"

@interface QuipApplicationDelegate : NSObject <NSApplicationDelegate>

- (const quip::GlobalSettings &)settings;
- (quip::ScriptHost*)scriptHost;

+ (QuipApplicationDelegate *)sharedDelegate;

@end

