#import <Cocoa/Cocoa.h>

#include "GlobalSettings.hpp"

@interface QuipApplicationDelegate : NSObject <NSApplicationDelegate>

- (const quip::GlobalSettings &)settings;

+ (QuipApplicationDelegate *)sharedDelegate;

@end

