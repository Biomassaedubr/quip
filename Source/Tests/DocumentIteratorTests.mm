#import <XCTest/XCTest.h>

#include "DocumentIterator.hpp"

#include "Document.hpp"

@interface DocumentIteratorTests : XCTestCase

@end

using namespace quip;

@implementation DocumentIteratorTests

- (void)testDereference {
  Document document("Q");
  DocumentIterator cursor = document.begin();
  
  XCTAssertEqual(*cursor, 'Q');
  XCTAssertEqual(cursor.location(), Location(0, 0));
}

- (void)testIncrement {
  Document document("Quip!");
  DocumentIterator cursor = document.begin();
  ++cursor;
  
  XCTAssertEqual(*cursor, 'u');
  XCTAssertEqual(cursor.location(), Location(1, 0));
}

- (void)testIncrementAcrossRow {
  Document document("A\nB");
  DocumentIterator cursor = document.begin();
  ++cursor;
  ++cursor;
  
  XCTAssertEqual(*cursor, 'B');
  XCTAssertEqual(cursor.location(), Location(0, 1));
}

- (void)testDecrement {
  Document document("Quip!");
  DocumentIterator cursor = document.end();
  --cursor;
  
  XCTAssertEqual(*cursor, '!');
}

- (void)testDecrementAcrossRow {
  Document document("A\nB");
  DocumentIterator cursor = document.end();
  --cursor;
  --cursor;
  
  XCTAssertEqual(*cursor, '\n');
}

- (void)testEquality {
  Document document("Quip!");
  DocumentIterator a = document.begin();
  DocumentIterator b = document.begin();

  XCTAssertTrue(a == b);
}

- (void)testInequality {
  Document document("Quip!");
  DocumentIterator a = document.begin();
  DocumentIterator b = document.begin();
  ++b;
  
  XCTAssertTrue(a != b);
}

@end
