#import <XCTest/XCTest.h>

#include "DocumentIterator.hpp"

#include "Document.hpp"

@interface DocumentIteratorTests : XCTestCase

@end

@implementation DocumentIteratorTests

- (void)testDereference {
  quip::Document document("Q");
  quip::DocumentIterator cursor = document.begin();
  
  XCTAssertEqual(*cursor, 'Q');
}

- (void)testIncrement {
  quip::Document document("Quip!");
  quip::DocumentIterator cursor = document.begin();
  ++cursor;
  
  XCTAssertEqual(*cursor, 'u');
}

- (void)testIncrementAcrossRow {
  quip::Document document("A\nB");
  quip::DocumentIterator cursor = document.begin();
  ++cursor;
  ++cursor;
  
  XCTAssertEqual(*cursor, 'B');
}

- (void)testDecrement {
  quip::Document document("Quip!");
  quip::DocumentIterator cursor = document.end();
  --cursor;
  
  XCTAssertEqual(*cursor, '!');
}

- (void)testDecrementAcrossRow {
  quip::Document document("A\nB");
  quip::DocumentIterator cursor = document.end();
  --cursor;
  --cursor;
  
  XCTAssertEqual(*cursor, '\n');
}

- (void)testEquality {
  quip::Document document("Quip!");
  quip::DocumentIterator a = document.begin();
  quip::DocumentIterator b = document.begin();

  XCTAssertTrue(a == b);
}

- (void)testInequality {
  quip::Document document("Quip!");
  quip::DocumentIterator a = document.begin();
  quip::DocumentIterator b = document.begin();
  ++b;
  
  XCTAssertTrue(a != b);
}

@end
