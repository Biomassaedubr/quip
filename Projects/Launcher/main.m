#import <Cocoa/Cocoa.h>

int main(int argc, const char** argv) {
  @autoreleasepool {
    // All arguments are currently assumed to be files, since Quip doesn't currently take
    // any other switches. Any path that isn't already absolute should be made such.
    NSString* workingDirectory = [[NSFileManager defaultManager] currentDirectoryPath];
    NSMutableArray* arguments = [[NSMutableArray alloc] initWithCapacity:argc - 1];
    for(int argument = 1; argument < argc; ++argument) {
      NSString* path = [NSString stringWithUTF8String:argv[argument]];
      if (![path isAbsolutePath]) {
        path = [workingDirectory stringByAppendingPathComponent:path];
      }
      
      [arguments addObject:path];
    }

    // Check to see if Quip is already running.
    NSWorkspace* workspace = [NSWorkspace sharedWorkspace];
    for (NSRunningApplication* application in [workspace runningApplications]) {
      if ([[application bundleIdentifier] isEqualToString:@"net.joshpetrie.Quip"]) {
        if (argc > 1) {
          // Open the specified files.
          NSString* executable = [[application bundleURL] path];
          for(NSString* path in arguments) {
            [workspace openFile:path withApplication:executable];
          }
        }
        
        // Just show the application.
        [application unhide];
        [application activateWithOptions:NSApplicationActivateAllWindows|NSApplicationActivateIgnoringOtherApps];
      
        return 0;
      }
    }
    
    // If the application isn't running, find it and launch it.
    NSString* path = [[[NSProcessInfo processInfo] environment] objectForKey:@"QUIP_LOCATION"];
    if (path == nil) {
      path = @"/Applications/Quip.app";
    }
    
    if (![[NSFileManager defaultManager] isExecutableFileAtPath:path]) {
      NSLog(@"The Quip bundle at '%@' is not executable.", path);
      return -1;
    }
    
    NSBundle* bundle = [NSBundle bundleWithPath:path];
    if (bundle == nil) {
      NSLog(@"The Quip bundle at '%@' could not be read.", path);
      return -1;
    }
  
    NSURL* url = [NSURL fileURLWithPath:path];
    NSDictionary* configuration = @{NSWorkspaceLaunchConfigurationArguments: arguments};
    NSError* error = nil;
    NSRunningApplication* result = [workspace launchApplicationAtURL:url options:NSWorkspaceLaunchDefault configuration:configuration error:&error];
    if (result == nil) {
      NSLog(@"The Quip bundle at '%@' could not be launched. %@", path, [error localizedDescription]);
      return -1;
    }
  }
  
  return 0;
}
