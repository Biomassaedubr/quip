#import "ApplicationDelegate.h"

@interface ApplicationDelegate ()

@end

@implementation ApplicationDelegate

- (BOOL)applicationShouldOpenUntitledFile:(NSApplication *)sender {
  return NO;
}

@end
