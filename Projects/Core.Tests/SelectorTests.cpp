#include "catch.hpp"

#include "Document.hpp"
#include "Location.hpp"
#include "Selection.hpp"
#include "Selector.hpp"

using namespace quip;

TEST_CASE("Select word on an empty document.", "Selector") {
  Document document;
  Optional<Selection> result = selectThisOrNextWord(document, Selection(0, 0, 2, 0));

  REQUIRE(!result.has_value());
}

TEST_CASE("Select word with a partial-word basis.", "Selector") {
  Document document("This is a test.");
  Optional<Selection> result = selectThisOrNextWord(document, Selection(1, 0, 2, 0));
  
  REQUIRE(result.has_value());
  REQUIRE(document.contents(*result) == "This ");
}

TEST_CASE("Select word with a partial-words basis.", "Selector") {
  Document document("This is a test.");
  Optional<Selection> result = selectThisOrNextWord(document, Selection(3, 0, 5, 0));
  
  REQUIRE(result.has_value());
  REQUIRE(document.contents(*result) == "This is ");
}

TEST_CASE("Select word with a full-word basis.", "Selector") {
  Document document("This is a test.");
  Optional<Selection> result = selectThisOrNextWord(document, Selection(0, 0, 4, 0));
  
  REQUIRE(result.has_value());
  REQUIRE(document.contents(*result) == "is ");
}

TEST_CASE("Select word with a full-words basis.", "Selector") {
  Document document("This is a test.");
  Optional<Selection> result = selectThisOrNextWord(document, Selection(0, 0, 7, 0));
  
  REQUIRE(result.has_value());
  REQUIRE(document.contents(*result) == "a ");
}

TEST_CASE("Select word with a partial-word basis and no trailing whitespace.", "Selector") {
  Document document("This is a test.");
  Optional<Selection> result = selectThisOrNextWord(document, Selection(11, 0));
  
  REQUIRE(result.has_value());
  REQUIRE(document.contents(*result) == "test");
}

TEST_CASE("Select word with a full-word basis and no trailing whitespace.", "Selector") {
  Document document("This is a test.");
  Optional<Selection> result = selectThisOrNextWord(document, Selection(10, 0, 13, 0));
  
  REQUIRE(result.has_value());
  REQUIRE(document.contents(*result) == ".");
}

TEST_CASE("Select word with a full-word basis and no words left.", "Selector") {
  Document document("This is a test.");
  Optional<Selection> result = selectThisOrNextWord(document, Selection(14, 0));
  
  REQUIRE(result.has_value());
  REQUIRE(document.contents(*result) == ".");
}

TEST_CASE("Select prior word on an empty document.", "Selector") {
  Document document;
  Optional<Selection> result = selectPriorWord(document, Selection(0, 0, 2, 0));
  
  REQUIRE(!result.has_value());
}

TEST_CASE("Select prior word with a partial-word basis.", "Selector") {
  Document document("This is not a test.");
  Optional<Selection> result = selectPriorWord(document, Selection(14, 0, 16, 0));
  
  REQUIRE(result.has_value());
  REQUIRE(document.contents(*result) == " test");
}

TEST_CASE("Select prior word with a partial-words basis.", "Selector") {
  Document document("This is not a test.");
  Optional<Selection> result = selectPriorWord(document, Selection(6, 0, 9, 0));
  
  REQUIRE(result.has_value());
  REQUIRE(document.contents(*result) == " is not");
}

TEST_CASE("Select prior word with a full-word basis.", "Selector") {
  Document document("This is not a test.");
  Optional<Selection> result = selectPriorWord(document, Selection(7, 0, 10, 0));
  
  REQUIRE(result.has_value());
  REQUIRE(document.contents(*result) == " is");
}

TEST_CASE("Select prior word with a full-words basis.", "Selector") {
  Document document("This is not a test.");
  Optional<Selection> result = selectPriorWord(document, Selection(7, 0, 12, 0));
  
  REQUIRE(result.has_value());
  REQUIRE(document.contents(*result) == " is");
}

