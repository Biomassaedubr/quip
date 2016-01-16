#import <XCTest/XCTest.h>

#include "SearchExpression.hpp"

@interface SearchExpressionTests : XCTestCase

@end

@implementation SearchExpressionTests

- (void)testConstructFromSimpleExpression {
  quip::SearchExpression expression("foo");
  
  XCTAssertTrue(expression.valid());
  XCTAssertEqual(expression.expression(), "foo");
}

- (void)testConstructExpressionWithTrailingClass {
  quip::SearchExpression expression("[a-z");
  
  XCTAssertFalse(expression.valid());
}

- (void)testConstructExpressionWithTrailingSlash {
  quip::SearchExpression expression("\\");
  
  XCTAssertFalse(expression.valid());
}

@end
