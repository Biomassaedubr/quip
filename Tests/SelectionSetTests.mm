#import <XCTest/XCTest.h>

#include "SelectionSet.hpp"

@interface SelectionSetTests : XCTestCase

@end

using namespace quip;

@implementation SelectionSetTests

- (void)testDefaultConstruction {
  SelectionSet empty;
  
  XCTAssertEqual(empty.count(), 0);
}

- (void)testSingleSelectionConstruction {
  Selection selection(Location(0, 0), Location(1, 0));
  SelectionSet set(selection);
  
  XCTAssertEqual(set.count(), 1);
  XCTAssertEqual(set.primary(), selection);
}

- (void)testMultipleSelectionConstruction {
  Selection a(Location(0, 0), Location(1, 0));
  Selection b(Location(0, 1), Location(1, 1));
  std::vector<Selection> selections { a, b };
  SelectionSet set(selections);
  
  XCTAssertEqual(set.count(), 2);
  XCTAssertEqual(set.primary(), a);
}

- (void)testIndexing {
  Selection a(Location(0, 0), Location(1, 0));
  Selection b(Location(0, 1), Location(1, 1));
  std::vector<Selection> selections { a, b };
  SelectionSet set(selections);
  
  XCTAssertEqual(set[0], a);
  XCTAssertEqual(set[1], b);
}

- (void)testIteration {
  Selection a(Location(0, 0), Location(1, 0));
  Selection b(Location(5, 5), Location(6, 5));
  std::vector<Selection> selections { a, b };
  SelectionSet set(selections);
  
  std::size_t index = 0;
  SelectionSetIterator cursor = set.begin();
  while (cursor != set.end()) {
    XCTAssertEqual(*cursor, selections[index]);
    ++cursor;
    ++index;
  }
}

- (void)testSelectionsAreSorted {
  Selection a(Location(0, 0), Location(0, 5));
  Selection b(Location(5, 10), Location(0, 8));
  Selection c(Location(10, 10), Location(0, 15));
  std::vector<Selection> selections { c, b, a };
  
  SelectionSet set(selections);
  SelectionSetIterator cursor = set.begin();
  XCTAssertEqual(*cursor, a);
  
  ++cursor;
  XCTAssertEqual(*cursor, b);
  
  ++cursor;
  XCTAssertEqual(*cursor, c);
}

- (void)testOverlappingSelectionsCollapse {
  Selection a(Location(1, 0), Location(10, 0));
  Selection b(Location(8, 0), Location(0, 5));
  Selection c(Location(0, 3), Location(0, 7));
  std::vector<Selection> selections { c, b, a };

  SelectionSet set(selections);
  XCTAssertEqual(set.count(), 1);
  
  SelectionSetIterator cursor = set.begin();
  XCTAssertEqual(cursor->origin(), Location(1, 0));
  XCTAssertEqual(cursor->extent(), Location(0, 7));
}

- (void)testPairsOfOverlappingSelectionsCollapse {
  Selection a(Location(1, 0), Location(10, 0));
  Selection b(Location(5, 0), Location(15, 0));
  Selection c(Location(0, 5), Location(15, 5));
  Selection d(Location(10, 5), Location(0, 10));
  std::vector<Selection> selections { d, c, b, a };
  
  SelectionSet set(selections);
  XCTAssertEqual(set.count(), 2);
  
  SelectionSetIterator cursor = set.begin();
  XCTAssertEqual(cursor->origin(), Location(1, 0));
  XCTAssertEqual(cursor->extent(), Location(15, 0));
  
  ++cursor;
  XCTAssertEqual(cursor->origin(), Location(0, 5));
  XCTAssertEqual(cursor->extent(), Location(0, 10));
}

@end
