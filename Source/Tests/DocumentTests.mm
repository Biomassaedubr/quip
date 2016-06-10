#import <XCTest/XCTest.h>

#include "Document.hpp"
#include "SelectionSet.hpp"

@interface DocumentTests : XCTestCase

@end

@implementation DocumentTests

using namespace quip;

- (void)testDefaultConstruction {
  Document document;
  
  XCTAssertEqual(document.rows(), 0);
}

- (void)testConstructFromSingleLineText {
  Document document("Hello, world.");
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.contents(), "Hello, world.");
}

- (void)testConstructFromMultipleLineText {
  Document document("Hello, world.\nThis is a test.\nThis file has three lines.");
  
  XCTAssertEqual(document.rows(), 3);
  XCTAssertEqual(document.row(0), "Hello, world.\n");
  XCTAssertEqual(document.row(1), "This is a test.\n");
  XCTAssertEqual(document.row(2), "This file has three lines.");
}

- (void)testConstructWithTrailingNewline {
  Document document("Hello, world.\n");
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.contents(), "Hello, world.\n");
}

- (void)testIsEmpty {
  Document empty;
  XCTAssertTrue(empty.isEmpty());
  
  Document filled("Hello, world!");
  XCTAssertFalse(filled.isEmpty());
}

- (void)testGetContentsOfSingleLineSelection {
  Document document("Hello, world!");
  std::string result = document.contents(Selection(Location(0, 0), Location(4, 0)));
  
  XCTAssertEqual(result, "Hello");
}

- (void)testGetContentsOfTwoLineSelection {
  Document document("Why hello\nworld!");
  std::string result = document.contents(Selection(Location(4, 0), Location(4, 1)));
  
  XCTAssertEqual(result, "hello\nworld");
}

- (void)testGetContentsOfFourLineSelection {
  Document document("!!A\nB\nC\nD!!");
  std::string result = document.contents(Selection(Location(2, 0), Location(0, 3)));
  
  XCTAssertEqual(result, "A\nB\nC\nD");
}

- (void)testSetPath {
  Document document;
  XCTAssertEqual(document.path(), "");
  
  document.setPath("~/test.txt");
  XCTAssertEqual(document.path(), "~/test.txt");
}

- (void)testInsertSingleLineIntoEmptyDocument {
  Document document;
  SelectionSet selections(Selection(Location(0, 0), Location(0, 0)));
  document.insert(selections, "Hello, world!");
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "Hello, world!");
}

- (void)testInsertMultipleLinesIntoEmptyDocument {
  Document document;
  SelectionSet selections(Selection(Location(0, 0), Location(0, 0)));
  document.insert(selections, "Hello, world!\nThis is a test.");
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.row(0), "Hello, world!\n");
  XCTAssertEqual(document.row(1), "This is a test.");
}

- (void)testInsertWithEmptyText {
  Document document("Hello, world!");
  std::vector<Selection> selections({
    Selection(Location(1, 0), Location(1, 0)),
    Selection(Location(4, 0), Location(4, 0))
  });
  SelectionSet set(selections);
  document.insert(set, "");
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "Hello, world!");
}

- (void)testInsertWithEmptySelections {
  Document document("Hello, world!");
  SelectionSet empty;
  document.insert(empty, "!");
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "Hello, world!");
}

- (void)testInsertSingleCharacter {
  Document document("AC");
  SelectionSet set(Selection(Location(1, 0), Location(1, 0)));
  SelectionSet results = document.insert(set, "_");

  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "A_C");
  XCTAssertEqual(results[0].origin(), Location(2, 0));
}

- (void)testInsertMultipleLines {
  Document document("hd");
  SelectionSet set(Selection(Location(1, 0), Location(1, 0)));
  SelectionSet results = document.insert(set, "ello\nworl");
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.row(0), "hello\n");
  XCTAssertEqual(document.row(1), "world");
  XCTAssertEqual(results[0].origin(), Location(4, 1));
}

- (void)testInsertSingleLineViaMultipleSelections {
  Document document("AC XZ");
  std::vector<Selection> selections({
    Selection(Location(1, 0), Location(1, 0)),
    Selection(Location(4, 0), Location(4, 0))
  });
  SelectionSet set(selections);
  SelectionSet results = document.insert(set, "_");
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "A_C X_Z");
  XCTAssertEqual(results[0].origin(), Location(2, 0));
  XCTAssertEqual(results[1].origin(), Location(6, 0));
}

- (void)testInsertMultipleLinesViaMultipleSelections {
  Document document("A\nBB\nCCC\nDDDD");
  std::vector<Selection> selections({
    Selection(Location(0, 0), Location(0, 0)),
    Selection(Location(1, 1), Location(1, 1)),
    Selection(Location(2, 2), Location(2, 2)),
    Selection(Location(3, 3), Location(3, 3))
  });
  SelectionSet set(selections);
  SelectionSet results = document.insert(set, "!\n_");
  
  XCTAssertEqual(document.rows(), 8);
  XCTAssertEqual(document.row(0), "!\n");
  XCTAssertEqual(document.row(1), "_A\n");
  XCTAssertEqual(document.row(2), "B!\n");
  XCTAssertEqual(document.row(3), "_B\n");
  XCTAssertEqual(document.row(4), "CC!\n");
  XCTAssertEqual(document.row(5), "_C\n");
  XCTAssertEqual(document.row(6), "DDD!\n");
  XCTAssertEqual(document.row(7), "_D");
  XCTAssertEqual(results[0].origin(), Location(1, 1));
  XCTAssertEqual(results[1].origin(), Location(1, 3));
  XCTAssertEqual(results[2].origin(), Location(1, 5));
  XCTAssertEqual(results[3].origin(), Location(1, 7));
}

- (void)testInsertNewlineViaMultipleSelections {
  Document document("A\nB\nC\nD");
  std::vector<Selection> selections({
    Selection(Location(0, 0), Location(0, 0)),
    Selection(Location(0, 1), Location(0, 1)),
    Selection(Location(0, 2), Location(0, 2)),
    Selection(Location(0, 3), Location(0, 3))
  });
  
  SelectionSet set(selections);
  SelectionSet results = document.insert(set, "\n");
  XCTAssertEqual(document.rows(), 8);
  XCTAssertEqual(document.row(0), "\n");
  XCTAssertEqual(document.row(1), "A\n");
  XCTAssertEqual(document.row(2), "\n");
  XCTAssertEqual(document.row(3), "B\n");
  XCTAssertEqual(document.row(4), "\n");
  XCTAssertEqual(document.row(5), "C\n");
  XCTAssertEqual(document.row(6), "\n");
  XCTAssertEqual(document.row(7), "D");
  XCTAssertEqual(results[0].origin(), Location(0, 1));
  XCTAssertEqual(results[1].origin(), Location(0, 3));
  XCTAssertEqual(results[2].origin(), Location(0, 5));
  XCTAssertEqual(results[3].origin(), Location(0, 7));
}

- (void)testInsertNewlineViaMultipleSelectionsOnTheSameLine {
  Document document("ABCD");
  std::vector<Selection> selections({
    Selection(Location(0, 0), Location(0, 0)),
    Selection(Location(1, 0), Location(1, 0)),
    Selection(Location(2, 0), Location(2, 0)),
    Selection(Location(3, 0), Location(3, 0))
  });
  
  SelectionSet set(selections);
  SelectionSet results = document.insert(set, "\n");
  XCTAssertEqual(document.rows(), 5);
  XCTAssertEqual(document.row(0), "\n");
  XCTAssertEqual(document.row(1), "A\n");
  XCTAssertEqual(document.row(2), "B\n");
  XCTAssertEqual(document.row(3), "C\n");
  XCTAssertEqual(document.row(4), "D");
  XCTAssertEqual(results[0].origin(), Location(0, 1));
  XCTAssertEqual(results[1].origin(), Location(0, 2));
  XCTAssertEqual(results[2].origin(), Location(0, 3));
  XCTAssertEqual(results[3].origin(), Location(0, 4));
}

- (void)testEraseWithEmptySelections {
  Document document("Hello, world!");
  SelectionSet empty;
  document.erase(empty);
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "Hello, world!");
}

- (void)testEraseSingleCharacter {
  Document document("AxBC");
  SelectionSet set(Selection(Location(1, 0), Location(1, 0)));
  SelectionSet results = document.erase(set);
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "ABC");
  XCTAssertEqual(results[0].origin(), Location(1, 0));
}

- (void)testEraseMultipleCharacters {
  Document document("AxBCDExF");
  std::vector<Selection> selections({
    Selection(Location(1, 0)),
    Selection(Location(6, 0))
  });
  SelectionSet set(selections);
  SelectionSet results = document.erase(set);
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "ABCDEF");
  XCTAssertEqual(results[0].origin(), Location(1, 0));
  XCTAssertEqual(results[1].origin(), Location(5, 0));
}

- (void)testEraseSingleLine {
  Document document("AA\nBB");
  SelectionSet set(Selection(Location(2, 0), Location(2, 0)));
  SelectionSet results = document.erase(set);

  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "AABB");
  XCTAssertEqual(results[0].origin(), Location(2, 0));
}

- (void)testEraseLastCharacterInDocument {
  Document document("Hello!");
  SelectionSet set(Selection(Location(5, 0), Location(5, 0)));
  SelectionSet results = document.erase(set);

  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "Hello");
  XCTAssertEqual(results[0].origin(), Location(4, 0));
}

- (void)testEraseMultipleLines {
  Document document("AA\nBB\nCC\nDD\n");
  std::vector<Selection> selections({
    Selection(Location(2, 0)),
    Selection(Location(2, 2)),
    Selection(Location(2, 3)),
  });
  SelectionSet set(selections);
  SelectionSet results = document.erase(set);
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.row(0), "AABB\n");
  XCTAssertEqual(document.row(1), "CCDD");
  XCTAssertEqual(results[0].origin(), Location(2, 0));
  XCTAssertEqual(results[1].origin(), Location(2, 1));
  XCTAssertEqual(results[2].origin(), Location(3, 1));
}

@end
