#include "catch.hpp"

#include "Document.hpp"
#include "Location.hpp"
#include "Selection.hpp"
#include "Selector.hpp"

using namespace quip;

TEST_CASE("One can select this word from the start of a document.", "[SelectorTests]") {
  Document document("Hello world!");
  Optional<Selection> result = selectWord(document, Selection(Location(0, 0)));
  
  REQUIRE(result->origin() == Location(0, 0));
  REQUIRE(result->extent() == Location(5, 0));
}

TEST_CASE("One can select this word from the start of a word.", "[SelectorTests]") {
  Document document("Hello world!");
  Optional<Selection> result = selectWord(document, Selection(Location(6, 0)));
  
  REQUIRE(result->origin() == Location(6, 0));
  REQUIRE(result->extent() == Location(10, 0));
}

TEST_CASE("One can select this word from the middle of a word.", "[SelectorTests]") {
  Document document("Hello world!");
  Optional<Selection> result = selectWord(document, Selection(Location(9, 0)));
  
  REQUIRE(result->origin() == Location(6, 0));
  REQUIRE(result->extent() == Location(10, 0));
}

TEST_CASE("Selecting this word covers the trailing whitespace.", "[SelectorTests]") {
  Document document("Hello world and friends!");
  Optional<Selection> result = selectWord(document, Selection(Location(6, 0)));
  
  REQUIRE(result->origin() == Location(6, 0));
  REQUIRE(result->extent() == Location(11, 0));
}

TEST_CASE("Selecting this word covers the trailing whitespace but not newlines.", "[SelectorTests]") {
  Document document("Hello world \nand friends!");
  Optional<Selection> result = selectWord(document, Selection(Location(6, 0)));
  
  REQUIRE(result->origin() == Location(6, 0));
  REQUIRE(result->extent() == Location(11, 0));
}

TEST_CASE("One can select the remaining word from a word.", "[SelectorTests]") {
  Document document("Hello world!");
  Optional<Selection> result = selectRemainingWord(document, Selection(Location(2, 0)));
  
  REQUIRE(result->origin() == Location(2, 0));
  REQUIRE(result->extent() == Location(5, 0));
}

TEST_CASE("One can select the prior word from a word.", "[SelectorTests]") {
  Document document("Hello world!");
  Optional<Selection> result = selectPriorWord(document, Selection(Location(7, 0)));
  
  REQUIRE(result->origin() == Location(0, 0));
  REQUIRE(result->extent() == Location(5, 0));
}

TEST_CASE("One can select the prior word from whitespace.", "[SelectorTests]") {
  Document document("Hello world!");
  Optional<Selection> result = selectPriorWord(document, Selection(Location(5, 0)));
  
  REQUIRE(result->origin() == Location(0, 0));
  REQUIRE(result->extent() == Location(5, 0));
}

TEST_CASE("One can select the prior word from across a newline.", "[SelectorTests]") {
  Document document("Hello world \nand friends!");
  Optional<Selection> result = selectPriorWord(document, Selection(Location(1, 1)));
  
  REQUIRE(result->origin() == Location(6, 0));
  REQUIRE(result->extent() == Location(11, 0));
}
