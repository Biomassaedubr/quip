#import <XCTest/XCTest.h>

#include "Document.hpp"

@interface DocumentTests : XCTestCase

@end

@implementation DocumentTests

- (void)testDefaultConstruction {
  quip::Document document;
  
  XCTAssertEqual(document.rows(), 0);
}

- (void)testConstructFromSingleLineText {
  quip::Document document("Hello, world.");
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.contents(), "Hello, world.");
}

- (void)testConstructFromMultipleLineText {
  quip::Document document("Hello, world.\nThis is a test.\nThis file has three lines.");
  
  XCTAssertEqual(document.rows(), 3);
  XCTAssertEqual(document.row(0), "Hello, world.\n");
  XCTAssertEqual(document.row(1), "This is a test.\n");
  XCTAssertEqual(document.row(2), "This file has three lines.");
}

- (void)testConstructWithTrailingNewline {
  quip::Document document("Hello, world.\n");
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.contents(), "Hello, world.\n");
}

- (void)testSetPath {
  quip::Document document;
  XCTAssertEqual(document.path(), "");
  
  document.setPath("~/test.txt");
  XCTAssertEqual(document.path(), "~/test.txt");
}

@end
