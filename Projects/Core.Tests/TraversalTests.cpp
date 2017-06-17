#include "catch.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "Traversal.hpp"

using namespace quip;

TEST_CASE("Advance-while using a document-order traversal with a predicate that fails immediately.", "Traversal") {
  Document document("ABCD");
  DocumentIterator iterator = Traversal::advanceWhile(document.begin(), [](char value) { return value == 'X'; });
  
  REQUIRE(iterator.location() == Location(0, 0));
}

TEST_CASE("Advance-while using a document-order traversal with a predicate that fails eventually.", "Traversal") {
  Document document("ABCD");
  DocumentIterator iterator = Traversal::advanceWhile(document.begin(), [](char value) { return value != 'D'; });
  
  REQUIRE(iterator.location() == Location(2, 0));
}

TEST_CASE("Advance-while using a document-order traversal with a predicate that never fails.", "Traversal") {
  Document document("ABCD");
  DocumentIterator iterator = Traversal::advanceWhile(document.begin(), [](char value) { return value != 'X'; });
  
  REQUIRE(iterator == document.end());
}
