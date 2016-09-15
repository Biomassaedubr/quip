#import <XCTest/XCTest.h>

#include "Extent.hpp"

@interface ExtentTests : XCTestCase

@end

static const float kAccuracy = 0.0000001f;

@implementation ExtentTests

using namespace quip;

- (void)testDefaultConstruction {
  Extent extent;
  
  XCTAssertEqualWithAccuracy(extent.width(), 0.0f, kAccuracy);
  XCTAssertEqualWithAccuracy(extent.height(), 0.0f, kAccuracy);
}

- (void)testParameterizedConstruction {
  Extent extent(1.4f, 2.7f);
  
  XCTAssertEqualWithAccuracy(extent.width(), 1.4f, kAccuracy);
  XCTAssertEqualWithAccuracy(extent.height(), 2.7f, kAccuracy);
}

@end
