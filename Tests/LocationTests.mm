#import <XCTest/XCTest.h>

#include "Location.hpp"

#include <utility>

@interface LocationTests : XCTestCase

@end

@implementation LocationTests

- (void)testDefaultConstruction {
  quip::Location location;
  
  XCTAssertEqual(location.column(), 0);
  XCTAssertEqual(location.row(), 0);
}

- (void)testParameterizedConstruction {
  quip::Location location(1, 10);
  
  XCTAssertEqual(location.column(), 1);
  XCTAssertEqual(location.row(), 10);
}

- (void)testAdjustBy {
  quip::Location location(1, 2);
  quip::Location result = location.adjustBy(10, 20);
  
  XCTAssertEqual(result.column(), 11);
  XCTAssertEqual(result.row(), 22);
}

- (void)testEquality {
  quip::Location a(1, 2);
  quip::Location b(1, 2);
  
  XCTAssertTrue(a == b);
}

- (void)testInequality {
  quip::Location a(1, 2);
  quip::Location b(5, 6);
  
  XCTAssertTrue(a != b);
}

- (void)testSwap {
  quip::Location a(1, 2);
  quip::Location b(3, 4);
  
  using std::swap;
  swap(a, b);
  
  XCTAssertEqual(a.column(), 3);
  XCTAssertEqual(b.column(), 1);
  XCTAssertEqual(a.row(), 4);
  XCTAssertEqual(b.row(), 2);
}

@end
