#import <XCTest/XCTest.h>

#include "SelectionSet.hpp"

@interface SelectionSetTests : XCTestCase

@end

@implementation SelectionSetTests

- (void)testSelectionsAreSorted {
  quip::Selection a(quip::Location(0, 0), quip::Location(0, 5));
  quip::Selection b(quip::Location(5, 10), quip::Location(0, 8));
  quip::Selection c(quip::Location(10, 10), quip::Location(0, 15));
  std::vector<quip::Selection> selections { c, b, a };
  
  quip::SelectionSet set(selections);
  
  quip::Selection * cursor = set.begin();
  XCTAssertEqual(*cursor, a);
  
  ++cursor;
  XCTAssertEqual(*cursor, b);
  
  ++cursor;
  XCTAssertEqual(*cursor, c);
}

@end
