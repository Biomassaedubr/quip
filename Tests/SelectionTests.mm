#import <XCTest/XCTest.h>

#include "Selection.hpp"

#include <utility>

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

- (void)testSwap {
  quip::Selection a(quip::Location(0, 0), quip::Location(1, 1));
  quip::Selection b(quip::Location(2, 2), quip::Location(3, 3));
  
  using std::swap;
  swap(a, b);
  
  XCTAssertEqual(a.origin(), quip::Location(2, 2));
  XCTAssertEqual(b.origin(), quip::Location(0, 0));
  XCTAssertEqual(a.extent(), quip::Location(3, 3));
  XCTAssertEqual(b.extent(), quip::Location(1, 1));
}

@end
