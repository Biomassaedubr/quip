#import <XCTest/XCTest.h>

#include "Document.hpp"
#include "Location.hpp"
#include "Selection.hpp"
#include "Selector.hpp"

#include <utility>

@interface SelectorTests : XCTestCase

@end

@implementation SelectorTests

using namespace quip;

- (void)testSelectThisWordFromStartOfDocument {
  Document document("Hello world!");
  Selection result = selectThisWord(document, Selection(Location(0, 0)));
  
  XCTAssertEqual(result.origin(), Location(0, 0));
  XCTAssertEqual(result.extent(), Location(5, 0));
}

- (void)testSelectThisWordFromStartOfWord {
  Document document("Hello world!");
  Selection result = selectThisWord(document, Selection(Location(6, 0)));
  
  XCTAssertEqual(result.origin(), Location(6, 0));
  XCTAssertEqual(result.extent(), Location(10, 0));
}

- (void)testSelectThisWordFromMiddleOfWord {
  Document document("Hello world!");
  Selection result = selectThisWord(document, Selection(Location(9, 0)));
  
  XCTAssertEqual(result.origin(), Location(6, 0));
  XCTAssertEqual(result.extent(), Location(10, 0));
}

- (void)testSelectThisWordCoversTrailingWhitespace {
  Document document("Hello world and friends!");
  Selection result = selectThisWord(document, Selection(Location(6, 0)));
  
  XCTAssertEqual(result.origin(), Location(6, 0));
  XCTAssertEqual(result.extent(), Location(11, 0));
}

- (void)testSelectThisWordCoversTrailingWhitespaceButNotNewlines {
  Document document("Hello world \nand friends!");
  Selection result = selectThisWord(document, Selection(Location(6, 0)));
  
  XCTAssertEqual(result.origin(), Location(6, 0));
  XCTAssertEqual(result.extent(), Location(11, 0));
}

- (void)testSelectRemainingWord {
  Document document("Hello world!");
  Selection result = selectRemainingWord(document, Selection(Location(2, 0)));

  XCTAssertEqual(result.origin(), Location(2, 0));
  XCTAssertEqual(result.extent(), Location(5, 0));
}

- (void)testSelectNextWordFromWord {
  Document document("Hello world!");
  Selection result = selectNextWord(document, Selection(Location(2, 0)));
  
  XCTAssertEqual(result.origin(), Location(6, 0));
  XCTAssertEqual(result.extent(), Location(10, 0));
}

- (void)testSelectNextWordFromWhitespace {
  Document document("Hello world!");
  Selection result = selectNextWord(document, Selection(Location(5, 0)));
  
  XCTAssertEqual(result.origin(), Location(6, 0));
  XCTAssertEqual(result.extent(), Location(10, 0));
}

- (void)testSelectNextWordAcrossNewline {
  Document document("Hello world \nand friends!");
  Selection result = selectNextWord(document, Selection(Location(6, 0)));
  
  XCTAssertEqual(result.origin(), Location(0, 1));
  XCTAssertEqual(result.extent(), Location(3, 1));
}

@end
