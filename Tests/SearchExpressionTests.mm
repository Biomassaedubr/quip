#import <XCTest/XCTest.h>

#include "SearchExpression.hpp"

@interface SearchExpressionTests : XCTestCase

@end

using namespace quip;

@implementation SearchExpressionTests

- (void)testConstructFromEmptyExpression {
  SearchExpression expression("");
  
  XCTAssertFalse(expression.valid());
}

- (void)testConstructFromSimpleExpression {
  SearchExpression expression("foo");
  
  XCTAssertTrue(expression.valid());
  XCTAssertEqual(expression.expression(), "foo");
}

- (void)testConstructExpressionWithTrailingClass {
  SearchExpression expression("[a-z");
  
  XCTAssertFalse(expression.valid());
}

- (void)testConstructExpressionWithTrailingSlash {
  SearchExpression expression("\\");
  
  XCTAssertFalse(expression.valid());
}

@end
