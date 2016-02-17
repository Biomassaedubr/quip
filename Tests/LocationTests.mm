#import <XCTest/XCTest.h>

#include "Location.hpp"

#include <utility>

@interface LocationTests : XCTestCase

@end

using namespace quip;

@implementation LocationTests

- (void)testDefaultConstruction {
  Location location;
  
  XCTAssertEqual(location.column(), 0);
  XCTAssertEqual(location.row(), 0);
}

- (void)testParameterizedConstruction {
  Location location(1, 10);
  
  XCTAssertEqual(location.column(), 1);
  XCTAssertEqual(location.row(), 10);
}

- (void)testAdjustBy {
  Location location(1, 2);
  Location result = location.adjustBy(10, 20);
  
  XCTAssertEqual(result.column(), 11);
  XCTAssertEqual(result.row(), 22);
}

- (void)testEquality {
  Location a(1, 2);
  Location b(1, 2);
  
  XCTAssertTrue(a == b);
}

- (void)testInequality {
  Location a(1, 2);
  Location b(5, 6);
  
  XCTAssertTrue(a != b);
}

- (void)testSwap {
  Location a(1, 2);
  Location b(3, 4);
  
  using std::swap;
  swap(a, b);
  
  XCTAssertEqual(a.column(), 3);
  XCTAssertEqual(b.column(), 1);
  XCTAssertEqual(a.row(), 4);
  XCTAssertEqual(b.row(), 2);
}

@end
