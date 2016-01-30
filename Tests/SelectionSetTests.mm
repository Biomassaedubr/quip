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

- (void)testOverlappingSelectionsCollapse {
  quip::Selection a(quip::Location(1, 0), quip::Location(10, 0));
  quip::Selection b(quip::Location(8, 0), quip::Location(0, 5));
  quip::Selection c(quip::Location(0, 3), quip::Location(0, 7));
  std::vector<quip::Selection> selections { c, b, a };

  quip::SelectionSet set(selections);
  XCTAssertEqual(set.count(), 1);
  
  quip::Selection * cursor = set.begin();
  XCTAssertEqual(cursor->origin(), quip::Location(1, 0));
  XCTAssertEqual(cursor->extent(), quip::Location(0, 7));
}

- (void)testPairsOfOverlappingSelectionsCollapse {
  quip::Selection a(quip::Location(1, 0), quip::Location(10, 0));
  quip::Selection b(quip::Location(5, 0), quip::Location(15, 0));
  quip::Selection c(quip::Location(0, 5), quip::Location(15, 5));
  quip::Selection d(quip::Location(10, 5), quip::Location(0, 10));
  std::vector<quip::Selection> selections { d, c, b, a };
  
  quip::SelectionSet set(selections);
  XCTAssertEqual(set.count(), 2);
  
  quip::Selection * cursor = set.begin();
  XCTAssertEqual(cursor->origin(), quip::Location(1, 0));
  XCTAssertEqual(cursor->extent(), quip::Location(15, 0));
  
  ++cursor;
  XCTAssertEqual(cursor->origin(), quip::Location(0, 5));
  XCTAssertEqual(cursor->extent(), quip::Location(0, 10));
}

@end
