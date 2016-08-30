#import <XCTest/XCTest.h>

#include "Signal.hpp"

@interface SignalTests : XCTestCase

@end

@implementation SignalTests

using namespace quip;

- (void)testConnectAndTriggerSingleListener {
  Signal<void ()> signal;
  bool success = false;
  signal.connect([&] { success = true; });
  signal.transmit();
  
  XCTAssertTrue(success);
}

- (void)testConnectAndTriggerMultipleListeners {
  Signal<void ()> signal;
  bool firstSuccess = false;
  bool secondSuccess = false;
  signal.connect([&] { firstSuccess = true; });
  signal.connect([&] { secondSuccess = true; });
  signal.transmit();
  
  XCTAssertTrue(firstSuccess);
  XCTAssertTrue(secondSuccess);
}

- (void)testDisconnectListenerFromFront {
  Signal<void ()> signal;
  bool firstSuccess = false;
  bool secondSuccess = false;
  bool thirdSuccess = false;
  std::uint32_t token = signal.connect([&] { firstSuccess = true; });
  signal.connect([&] { secondSuccess = true; });
  signal.connect([&] { thirdSuccess = true; });
  signal.disconnect(token);
  
  signal.transmit();
  XCTAssertFalse(firstSuccess);
  XCTAssertTrue(secondSuccess);
  XCTAssertTrue(thirdSuccess);
}

- (void)testDisconnectListenerFromMiddle {
  Signal<void ()> signal;
  bool firstSuccess = false;
  bool secondSuccess = false;
  bool thirdSuccess = false;
  signal.connect([&] { firstSuccess = true; });
  std::uint32_t token = signal.connect([&] { secondSuccess = true; });
  signal.connect([&] { thirdSuccess = true; });
  signal.disconnect(token);
  
  signal.transmit();
  XCTAssertTrue(firstSuccess);
  XCTAssertFalse(secondSuccess);
  XCTAssertTrue(thirdSuccess);
}

- (void)testDisconnectListenerFromEnd {
  Signal<void ()> signal;
  bool firstSuccess = false;
  bool secondSuccess = false;
  bool thirdSuccess = false;
  signal.connect([&] { firstSuccess = true; });
  signal.connect([&] { secondSuccess = true; });
  std::uint32_t token = signal.connect([&] { thirdSuccess = true; });
  signal.disconnect(token);
  
  signal.transmit();
  XCTAssertTrue(firstSuccess);
  XCTAssertTrue(secondSuccess);
  XCTAssertFalse(thirdSuccess);
  
  // Connect another listener to ensure the tail pointer is correctly
  // updated by the disconnection.
  signal.connect([&] { thirdSuccess = true; });
  signal.transmit();
  XCTAssertTrue(thirdSuccess);
}

- (void)testDisconnectOnlyListener {
  Signal<void ()> signal;
  std::uint32_t token = signal.connect([&] { XCTFail("Listener should not be called."); });
  signal.disconnect(token);
  signal.transmit();
  
  // Connect another listener to ensure the tail pointer is correctly
  // updated by the disconnection.
  bool success = false;
  signal.connect([&] { success = true; });
  signal.transmit();
  XCTAssertTrue(success);
}

@end
