#import <XCTest/XCTest.h>

#include "Document.hpp"
#include "SelectionSet.hpp"

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

- (void)testInsertSingleLine {
  quip::Document document("AC");
  quip::SelectionSet set(quip::Selection(quip::Location(1, 0), quip::Location(1, 0)));
  quip::SelectionSet results = document.insert(set, "_");

  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "A_C");
  XCTAssertEqual(results[0].origin(), quip::Location(2, 0));
}

- (void)testInsertMultipleLines {
  quip::Document document("hd");
  quip::SelectionSet set(quip::Selection(quip::Location(1, 0), quip::Location(1, 0)));
  quip::SelectionSet results = document.insert(set, "ello\nworl");
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.row(0), "hello\n");
  XCTAssertEqual(document.row(1), "world");
  XCTAssertEqual(results[0].origin(), quip::Location(4, 1));
}

- (void)testInsertSingleLineViaMultipleSelections {
  quip::Document document("AC XZ");
  std::vector<quip::Selection> selections({
    quip::Selection(quip::Location(1, 0), quip::Location(1, 0)),
    quip::Selection(quip::Location(4, 0), quip::Location(4, 0))
  });
  quip::SelectionSet set(selections);
  quip::SelectionSet results = document.insert(set, "_");
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "A_C X_Z");
  XCTAssertEqual(results[0].origin(), quip::Location(2, 0));
  XCTAssertEqual(results[1].origin(), quip::Location(6, 0));
}

- (void)testInsertMultipleLinesViaMultipleSelections {
  quip::Document document("A\nBB\nCCC\nDDDD");
  std::vector<quip::Selection> selections({
    quip::Selection(quip::Location(0, 0), quip::Location(0, 0)),
    quip::Selection(quip::Location(1, 1), quip::Location(1, 1)),
    quip::Selection(quip::Location(2, 2), quip::Location(2, 2)),
    quip::Selection(quip::Location(3, 3), quip::Location(3, 3))
  });
  quip::SelectionSet set(selections);
  quip::SelectionSet results = document.insert(set, "!\n_");
  
  XCTAssertEqual(document.rows(), 8);
  XCTAssertEqual(document.row(0), "!\n");
  XCTAssertEqual(document.row(1), "_A\n");
  XCTAssertEqual(document.row(2), "B!\n");
  XCTAssertEqual(document.row(3), "_B\n");
  XCTAssertEqual(document.row(4), "CC!\n");
  XCTAssertEqual(document.row(5), "_C\n");
  XCTAssertEqual(document.row(6), "DDD!\n");
  XCTAssertEqual(document.row(7), "_D");
  XCTAssertEqual(results[0].origin(), quip::Location(1, 1));
  XCTAssertEqual(results[1].origin(), quip::Location(1, 3));
  XCTAssertEqual(results[2].origin(), quip::Location(1, 5));
  XCTAssertEqual(results[3].origin(), quip::Location(1, 7));
}

- (void)testInsertNewlineViaMultipleSelections {
  quip::Document document("A\nB\nC\nD");
  std::vector<quip::Selection> selections({
    quip::Selection(quip::Location(0, 0), quip::Location(0, 0)),
    quip::Selection(quip::Location(0, 1), quip::Location(0, 1)),
    quip::Selection(quip::Location(0, 2), quip::Location(0, 2)),
    quip::Selection(quip::Location(0, 3), quip::Location(0, 3))
  });
  
  quip::SelectionSet set(selections);
  quip::SelectionSet results = document.insert(set, "\n");
  XCTAssertEqual(document.rows(), 8);
  XCTAssertEqual(document.row(0), "\n");
  XCTAssertEqual(document.row(1), "A\n");
  XCTAssertEqual(document.row(2), "\n");
  XCTAssertEqual(document.row(3), "B\n");
  XCTAssertEqual(document.row(4), "\n");
  XCTAssertEqual(document.row(5), "C\n");
  XCTAssertEqual(document.row(6), "\n");
  XCTAssertEqual(document.row(7), "D");
  XCTAssertEqual(results[0].origin(), quip::Location(0, 1));
  XCTAssertEqual(results[1].origin(), quip::Location(0, 3));
  XCTAssertEqual(results[2].origin(), quip::Location(0, 5));
  XCTAssertEqual(results[3].origin(), quip::Location(0, 7));
}

- (void)testInsertNewlineViaMultipleSelectionsOnTheSameLine {
  quip::Document document("ABCD");
  std::vector<quip::Selection> selections({
    quip::Selection(quip::Location(0, 0), quip::Location(0, 0)),
    quip::Selection(quip::Location(1, 0), quip::Location(1, 0)),
    quip::Selection(quip::Location(2, 0), quip::Location(2, 0)),
    quip::Selection(quip::Location(3, 0), quip::Location(3, 0))
  });
  
  quip::SelectionSet set(selections);
  quip::SelectionSet results = document.insert(set, "\n");
  XCTAssertEqual(document.rows(), 5);
  XCTAssertEqual(document.row(0), "\n");
  XCTAssertEqual(document.row(1), "A\n");
  XCTAssertEqual(document.row(2), "B\n");
  XCTAssertEqual(document.row(3), "C\n");
  XCTAssertEqual(document.row(4), "D");
  XCTAssertEqual(results[0].origin(), quip::Location(0, 1));
  XCTAssertEqual(results[1].origin(), quip::Location(0, 2));
  XCTAssertEqual(results[2].origin(), quip::Location(0, 3));
  XCTAssertEqual(results[3].origin(), quip::Location(0, 4));
}

@end
