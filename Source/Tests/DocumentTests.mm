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
  XCTAssertTrue(document.isMissingTrailingNewline());
}

- (void)testConstructFromSingleLineText {
  Document document("ABCD");
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.contents(), "ABCD");
  XCTAssertTrue(document.isMissingTrailingNewline());
}

- (void)testConstructFromMultipleLineText {
  Document document("ABCD\nEFGH\nIJKL");
  
  XCTAssertEqual(document.rows(), 3);
  XCTAssertEqual(document.row(0), "ABCD\n");
  XCTAssertEqual(document.row(1), "EFGH\n");
  XCTAssertEqual(document.row(2), "IJKL");
}

- (void)testConstructWithTrailingNewline {
  Document document("ABCD\n");
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.contents(), "ABCD\n");
  XCTAssertFalse(document.isMissingTrailingNewline());
}

- (void)testIsEmpty {
  Document empty;
  XCTAssertTrue(empty.isEmpty());
  
  Document filled("ABCD");
  XCTAssertFalse(filled.isEmpty());
}

- (void)testDistanceBetweenSameLocation {
  Document document("ABCD");
  std::int64_t result = document.distance(Location(0, 0), Location(0, 0));
  
  XCTAssertEqual(result, 0);
}

- (void)testDistanceBetweenIncreasingLocationsOnSameRow {
  Document document("ABCD");
  std::int64_t result = document.distance(Location(1, 0), Location(3, 0));
  
  XCTAssertEqual(result, 2);
}

- (void)testDistanceBetweenDecreasingLocationsOnSameRow {
  Document document("ABCD");
  std::int64_t result = document.distance(Location(3, 0), Location(1, 0));
  
  XCTAssertEqual(result, -2);
}

- (void)testDistanceBetweenIncreasingLocationsOnDifferentRows {
  Document document("ABCD\nEFGH\nIJKL\n");
  std::int64_t result = document.distance(Location(1, 0), Location(2, 2));
  
  XCTAssertEqual(result, 11);
}

- (void)testDistanceBetweenDecreasingLocationsOnDifferentRows {
  Document document("ABCD\nEFGH\nIJKL\n");
  std::int64_t result = document.distance(Location(2, 2), Location(1, 0));
  
  XCTAssertEqual(result, -11);
}

- (void)testGetContentsOfEntireDocument {
  Document document("ABCD\nEFGH\n");
  std::string result = document.contents();
  
  XCTAssertEqual(result, "ABCD\nEFGH\n");
}

- (void)testGetContentsOfSelection {
  Document document("ABCD\nEFGH\n");
  std::string result = document.contents(Selection(Location(2, 0), Location(1, 1)));
  
  XCTAssertEqual(result, "CD\nEF");
}

- (void)testGetContentsOfSelectionSet {
  Document document("ABCD\nEFGH\nIJKL\n");
  SelectionSet selections({
    Selection(Location(3, 0), Location(0, 1)),
    Selection(Location(3, 1), Location(0, 2))
  });
  std::vector<std::string> result = document.contents(selections);
  
  XCTAssertEqual(result.size(), 2);
  XCTAssertEqual(result[0], "D\nE");
  XCTAssertEqual(result[1], "H\nI");
}

- (void)testSetPath {
  Document document;
  XCTAssertEqual(document.path(), "");
  
  document.setPath("~/path.txt");
  XCTAssertEqual(document.path(), "~/path.txt");
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

#pragma mark Erase Tests

- (void)testEraseEmptySelections {
  Document document("ABCD\n");
  SelectionSet empty;
  document.erase(empty);

  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.row(0), "ABCD\n");
}

- (void)testEraseSingleCharacterViaSingleSelection {
  Document document("ABCD\n");
  SelectionSet result = document.erase(Selection(Location(1, 0)));
  
  XCTAssertEqual(document.row(0), "ACD\n");
  XCTAssertEqual(result.count(), 1);
  XCTAssertEqual(result.primary().origin(), Location(1, 0));
}

- (void)testEraseMultipleCharactersViaSingleSelection {
  Document document("ABCD\n");
  SelectionSet result = document.erase(Selection(Location(1, 0), Location(2, 0)));
  
  XCTAssertEqual(document.row(0), "AD\n");
  XCTAssertEqual(result.count(), 1);
  XCTAssertEqual(result.primary().origin(), Location(1, 0));
}

- (void)testEraseSingleLineViaSingleSelection {
  Document document("ABCD\nEFGH\n");
  SelectionSet result = document.erase(Selection(Location(0, 0), Location(4, 0)));
  
  XCTAssertEqual(document.row(0), "EFGH\n");
  XCTAssertEqual(result.count(), 1);
  XCTAssertEqual(result.primary().origin(), Location(0, 0));
}

- (void)testEraseMultipleLinesViaSingleSelection {
  Document document("ABCD\nEFGH\nHIJK\n");
  SelectionSet result = document.erase(Selection(Location(0, 0), Location(4, 1)));
  
  XCTAssertEqual(document.row(0), "HIJK\n");
  XCTAssertEqual(result.count(), 1);
  XCTAssertEqual(result.primary().origin(), Location(0, 0));
}

- (void)testEraseMultipleCharactersViaMultipleSelections {
  Document document("ABCDEFGH\n");
  SelectionSet selections({
    Selection(Location(2, 0)),
    Selection(Location(4, 0), Location(5, 0)),
    Selection(Location(7, 0))
  });
  SelectionSet result = document.erase(selections);
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.row(0), "ABDG\n");
  XCTAssertEqual(result.count(), 3);
  XCTAssertEqual(result[0].origin(), Location(2, 0));
  XCTAssertEqual(result[1].origin(), Location(3, 0));
  XCTAssertEqual(result[2].origin(), Location(4, 0));
}

- (void)testEraseMultipleCharactersAcrossMultipleLinesViaMultipleSelections {
  Document document("ABCDEFGH\nIJKLMNOP\n");
  SelectionSet selections({
    Selection(Location(1, 0), Location(2, 0)),
    Selection(Location(6, 0), Location(1, 1)),
    Selection(Location(5, 1), Location(6, 1))
  });
  SelectionSet result = document.erase(selections);
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.row(0), "ADEFKLMP\n");
  XCTAssertEqual(result.count(), 3);
  XCTAssertEqual(result[0].origin(), Location(1, 0));
  XCTAssertEqual(result[1].origin(), Location(4, 0));
  XCTAssertEqual(result[2].origin(), Location(7, 0));
}

- (void)testEraseSingleNewline {
  Document document("ABCD\nEFGH\n");
  SelectionSet result = document.erase(Selection(Location(4, 0)));
  
  XCTAssertEqual(document.row(0), "ABCDEFGH\n");
  XCTAssertEqual(result.count(), 1);
  XCTAssertEqual(result.primary().origin(), Location(4, 0));
}

- (void)testEraseMultipleNewlines {
  Document document("AB\nCD\nEF\nGH\n");
  SelectionSet selections({
    Selection(Location(2, 0)),
    Selection(Location(2, 2)),
    Selection(Location(2, 3))
  });
  SelectionSet result = document.erase(selections);
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.row(0), "ABCD\n");
  XCTAssertEqual(document.row(1), "EFGH");
  XCTAssertEqual(result.count(), 3);
  XCTAssertEqual(result[0].origin(), Location(2, 0));
  XCTAssertEqual(result[1].origin(), Location(2, 1));
  XCTAssertEqual(result[2].origin(), Location(3, 1));
}

- (void)testEraseLastCharacterInDocument {
  Document document("ABCD");
  SelectionSet result = document.erase(Selection(Location(3, 0)));

  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "ABC");
  XCTAssertEqual(result.primary().origin(), Location(2, 0));
}

- (void)testEraseLastCharacterOfDocument {
  Document document("A");
  SelectionSet result = document.erase(Selection(Location(0, 0)));
  
  XCTAssertEqual(document.rows(), 0);
  XCTAssertEqual(result.primary().origin(), Location(0, 0));
}

@end
