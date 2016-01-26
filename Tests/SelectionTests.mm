#import <XCTest/XCTest.h>

#include "Selection.hpp"

@interface SelectionTests : XCTestCase

@end

@implementation SelectionTests

- (void)testEquality {
  quip::Selection a(quip::Location(0, 0), quip::Location(10, 5));
  quip::Selection b(quip::Location(0, 0), quip::Location(10, 5));
  quip::Selection c(quip::Location(1, 0), quip::Location(1, 5));

  XCTAssertTrue(a == b);
  XCTAssertFalse(b == c);
}

- (void)testInequality {
  quip::Selection a(quip::Location(0, 0), quip::Location(10, 5));
  quip::Selection b(quip::Location(0, 0), quip::Location(10, 5));
  quip::Selection c(quip::Location(1, 0), quip::Location(1, 5));
  
  XCTAssertFalse(a != b);
  XCTAssertTrue(b != c);
}

@end
