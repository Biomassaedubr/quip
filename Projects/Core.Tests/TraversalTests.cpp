#include "catch.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "Traversal.hpp"

using namespace quip;

TEST_CASE("Construct a document-order traversal.", "Traversal") {
  Document document("ABCD");
  Traversal traversal = Traversal::documentOrder(document);
  
  REQUIRE(traversal.advanceTo() == document.end());
  REQUIRE(traversal.retreatTo() == document.begin());
}

TEST_CASE("Advance using a document-order traversal.", "Traversal") {
  Document document("ABCD");
  Traversal traversal = Traversal::documentOrder(document);
  DocumentIterator iterator = traversal.advance(document.begin());
  
  REQUIRE(iterator.location() == Location(1, 0));
}

TEST_CASE("Retreat using a document-order traversal.", "Traversal") {
  Document document("ABCD");
  Traversal traversal = Traversal::documentOrder(document);
  DocumentIterator iterator = traversal.retreat(document.end());
  
  REQUIRE(iterator.location() == Location(3, 0));
}

TEST_CASE("Construct a reverse-document-order traversal.", "Traversal") {
  Document document("ABCD");
  Traversal traversal = Traversal::reverseDocumentOrder(document);
  
  REQUIRE(traversal.advanceTo() == document.begin());
  REQUIRE(traversal.retreatTo() == document.end());
}

TEST_CASE("Advance using a reverse-document-order traversal.", "Traversal") {
  Document document("ABCD");
  Traversal traversal = Traversal::reverseDocumentOrder(document);
  DocumentIterator iterator = traversal.advance(document.end());
  
  REQUIRE(iterator.location() == Location(3, 0));
}

TEST_CASE("Retreat using a reverse-document-order traversal.", "Traversal") {
  Document document("ABCD");
  Traversal traversal = Traversal::reverseDocumentOrder(document);
  DocumentIterator iterator = traversal.retreat(document.begin());
  
  REQUIRE(iterator.location() == Location(1, 0));
}

TEST_CASE("Advance-while using a document-order traversal with a predicate that fails immediately.", "Traversal") {
  Document document("ABCD");
  Traversal traversal = Traversal::documentOrder(document);
  DocumentIterator iterator = traversal.advanceWhile(document.begin(), [](char value) { return value == 'X'; });
  
  REQUIRE(iterator.location() == Location(0, 0));
}

TEST_CASE("Advance-while using a document-order traversal with a predicate that fails eventually.", "Traversal") {
  Document document("ABCD");
  Traversal traversal = Traversal::documentOrder(document);
  DocumentIterator iterator = traversal.advanceWhile(document.begin(), [](char value) { return value != 'D'; });
  
  REQUIRE(iterator.location() == Location(2, 0));
}

TEST_CASE("Advance-while using a document-order traversal with a predicate that never fails.", "Traversal") {
  Document document("ABCD");
  Traversal traversal = Traversal::documentOrder(document);
  DocumentIterator iterator = traversal.advanceWhile(document.begin(), [](char value) { return value != 'X'; });
  
  REQUIRE(iterator == document.end());
}
