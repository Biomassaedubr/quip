#import <XCTest/XCTest.h>

#include "Coordinate.hpp"

@interface CoordinateTests : XCTestCase

@end

static const float kAccuracy = 0.0000001f;

@implementation CoordinateTests

using namespace quip;

- (void)testDefaultConstruction {
  Coordinate coordinate;
  
  XCTAssertEqualWithAccuracy(coordinate.x, 0.0f, kAccuracy);
  XCTAssertEqualWithAccuracy(coordinate.y, 0.0f, kAccuracy);
}

- (void)testParameterizedConstruction {
  Coordinate coordinate(-10.5f, 91.2f);
  
  XCTAssertEqualWithAccuracy(coordinate.x, -10.5f, kAccuracy);
  XCTAssertEqualWithAccuracy(coordinate.y, 91.2f, kAccuracy);
}

@end
