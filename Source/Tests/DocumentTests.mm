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
  
  XCTAssertEqual(document.rows(), 1);
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

- (void)testGetContentsOfEntireDocumentForEmptyDocument {
  Document document;
  std::string result = document.contents();
  
  XCTAssertEqual(result, "");
}

- (void)testGetContentsOfSelection {
  Document document("ABCD\nEFGH\n");
  std::string result = document.contents(Selection(Location(2, 0), Location(1, 1)));
  
  XCTAssertEqual(result, "CD\nEF");
}

- (void)testGetContentsOfSelectionForEmptyDocument {
  Document document;
  std::string result = document.contents(Selection(Location(0, 0)));
  
  XCTAssertEqual(result, "");
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

- (void)testGetContentsOfSelectionSetForEmptyDocument {
  Document document;
  SelectionSet selections(Selection(Location(0, 0)));
  std::vector<std::string> result = document.contents(selections);
  
  XCTAssertEqual(result.size(), 0);
}

- (void)testSetPath {
  Document document;
  XCTAssertEqual(document.path(), "");
  
  document.setPath("~/path.txt");
  XCTAssertEqual(document.path(), "~/path.txt");
}

- (void)testInsertTextIntoEmptyDocument {
  Document document;
  SelectionSet result = document.insert(Selection(Location(0, 0)), "ABCD\nEFGH\n");
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.row(0), "ABCD\n");
  XCTAssertEqual(document.row(1), "EFGH\n");
  XCTAssertEqual(result.count(), 1);
  XCTAssertEqual(result.primary().origin(), Location(5, 1));
}

- (void)testInsertTextViaSingleSelection {
  Document document("ABD\n");
  SelectionSet result = document.insert(Selection(Location(2, 0)), "C");

  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "ABCD\n");
  XCTAssertEqual(result.count(), 1);
  XCTAssertEqual(result.primary().origin(), Location(3, 0));
}

- (void)testInsertTextViaMultipleSelections {
  Document document("ADF\n");
  std::vector<std::string> text({
    "BC",
    "E",
    "GH"
  });
  SelectionSet selections({
    Selection(Location(1, 0)),
    Selection(Location(2, 0)),
    Selection(Location(3, 0)),
    
  });
  SelectionSet result = document.insert(selections, text);
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "ABCDEFGH\n");
  XCTAssertEqual(result.count(), 3);
  XCTAssertEqual(result[0].origin(), Location(3, 0));
  XCTAssertEqual(result[1].origin(), Location(5, 0));
  XCTAssertEqual(result[2].origin(), Location(8, 0));
}

- (void)testInsertTextWithLeadingNewlinesViaSingleSelection {
  Document document("ABCD\n");
  document.insert(Selection(Location(4, 0)), "\nEFGH");
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.row(0), "ABCD\n");
  XCTAssertEqual(document.row(1), "EFGH\n");
}

- (void)testInsertTextWithTrailingNewlinesViaSingleSelection {
  Document document("ABCD\n");
  SelectionSet result = document.insert(Selection(Location(4, 0)), "EFGH\n");
  
  XCTAssertEqual(document.rows(), 2);
  XCTAssertEqual(document.row(0), "ABCDEFGH\n");
  XCTAssertEqual(document.row(1), "\n");
  XCTAssertEqual(result.count(), 1);
  XCTAssertEqual(result.primary().origin(), Location(0, 1));
}

- (void)testInsertTextWithInternalNewlinesViaSingleSelection {
  Document document("AB\nKL\n");
  SelectionSet result = document.insert(Selection(Location(2, 0)), "CD\nEFGH\nIJ");
  
  XCTAssertEqual(document.rows(), 4);
  XCTAssertEqual(document.row(0), "ABCD\n");
  XCTAssertEqual(document.row(1), "EFGH\n");
  XCTAssertEqual(document.row(2), "IJ\n");
  XCTAssertEqual(document.row(3), "KL\n");
  XCTAssertEqual(result.count(), 1);
  XCTAssertEqual(result.primary().origin(), Location(2, 2));
}

- (void)testEraseEmptySelections {
  Document document("ABCD\n");
  SelectionSet empty;
  document.erase(empty);

  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "ABCD\n");
}

- (void)testEraseSingleCharacterViaSingleSelection {
  Document document("ABCD\n");
  SelectionSet result = document.erase(Selection(Location(1, 0)));
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "ACD\n");
  XCTAssertEqual(result.count(), 1);
  XCTAssertEqual(result.primary().origin(), Location(1, 0));
}

- (void)testEraseMultipleCharactersViaSingleSelection {
  Document document("ABCD\n");
  SelectionSet result = document.erase(Selection(Location(1, 0), Location(2, 0)));
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "AD\n");
  XCTAssertEqual(result.count(), 1);
  XCTAssertEqual(result.primary().origin(), Location(1, 0));
}

- (void)testEraseSingleLineViaSingleSelection {
  Document document("ABCD\nEFGH\n");
  SelectionSet result = document.erase(Selection(Location(0, 0), Location(4, 0)));
  
  XCTAssertEqual(document.rows(), 1);
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
  
  XCTAssertEqual(document.rows(), 1);
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
  
  XCTAssertEqual(document.rows(), 1);
  XCTAssertEqual(document.row(0), "ADEFKLMP\n");
  XCTAssertEqual(result.count(), 3);
  XCTAssertEqual(result[0].origin(), Location(1, 0));
  XCTAssertEqual(result[1].origin(), Location(4, 0));
  XCTAssertEqual(result[2].origin(), Location(7, 0));
}

- (void)testEraseMultipleCharactersAcrossMultipleDisjointLinesViaMultipleSelections {
  Document document("ABCDEFGH\nIJKLMNOP\nQRSTUVWX\n");
  SelectionSet selections({
    Selection(Location(0, 0)),
    Selection(Location(2, 0), Location(4, 0)),
    Selection(Location(1, 2)),
    Selection(Location(3, 2), Location(5, 2))
  });
  SelectionSet result = document.erase(selections);
  
  XCTAssertEqual(document.rows(), 3);
  XCTAssertEqual(document.row(0), "BFGH\n");
  XCTAssertEqual(document.row(1), "IJKLMNOP\n");
  XCTAssertEqual(document.row(2), "QSWX\n");
  XCTAssertEqual(result.count(), 4);
  XCTAssertEqual(result[0].origin(), Location(0, 0));
  XCTAssertEqual(result[1].origin(), Location(1, 0));
  XCTAssertEqual(result[2].origin(), Location(1, 2));
  XCTAssertEqual(result[3].origin(), Location(2, 2));
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
