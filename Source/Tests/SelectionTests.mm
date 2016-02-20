#import <XCTest/XCTest.h>

#include "Selection.hpp"

#include <utility>

@interface SelectionTests : XCTestCase

@end

@implementation SelectionTests

using namespace quip;

- (void)testConstructFromSingleLocation {
  Selection selection(Location(1, 2));
  
  XCTAssertEqual(selection.origin(), selection.extent());
}

- (void)testEquality {
  Selection a(Location(0, 0), Location(10, 5));
  Selection b(Location(0, 0), Location(10, 5));
  Selection c(Location(1, 0), Location(1, 5));

  XCTAssertTrue(a == b);
  XCTAssertFalse(b == c);
}

- (void)testInequality {
  Selection a(Location(0, 0), Location(10, 5));
  Selection b(Location(0, 0), Location(10, 5));
  Selection c(Location(1, 0), Location(1, 5));
  
  XCTAssertFalse(a != b);
  XCTAssertTrue(b != c);
}

- (void)testSwap {
  Selection a(Location(0, 0), Location(1, 1));
  Selection b(Location(2, 2), Location(3, 3));
  
  using std::swap;
  swap(a, b);
  
  XCTAssertEqual(a.origin(), Location(2, 2));
  XCTAssertEqual(b.origin(), Location(0, 0));
  XCTAssertEqual(a.extent(), Location(3, 3));
  XCTAssertEqual(b.extent(), Location(1, 1));
}

@end
