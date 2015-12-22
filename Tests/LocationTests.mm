#import <XCTest/XCTest.h>

#include "Location.hpp"

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

@end
