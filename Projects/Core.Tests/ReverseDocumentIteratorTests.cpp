#include "catch.hpp"

#include "Document.hpp"
#include "ReverseDocumentIterator.hpp"

using namespace quip;

TEST_CASE("Reverse document begin iterator location is the last valid location in the document.", "ReverseDocumentIterator") {
  Document document("A\nB");
  ReverseDocumentIterator cursor = document.rbegin();
  
  REQUIRE(cursor.location() == Location(0, 1));
}

TEST_CASE("Reverse document end iterator location is invalid.", "ReverseDocumentIterator") {
  Document document("A\nB");
  ReverseDocumentIterator cursor = document.rend();
  REQUIRE(cursor.location() == Location::invalid());
  
  cursor = document.rbegin();
  ++cursor;
  ++cursor;
  ++cursor;
  REQUIRE(cursor == document.rend());
}

TEST_CASE("Reverse document iterators can be dereferenced.", "ReverseDocumentIterator") {
  Document document("Q");
  ReverseDocumentIterator cursor = document.rbegin();
  
  REQUIRE(*cursor == 'Q');
  REQUIRE(cursor.location() == Location(0, 0));
}

TEST_CASE("Reverse document iterators can be pre-incremented.", "ReverseDocumentIterator") {
  Document document("Quip!");
  ReverseDocumentIterator cursor = document.rbegin();
  ++cursor;
  
  REQUIRE(*cursor == 'p');
  REQUIRE(cursor.location() == Location(3, 0));
}

TEST_CASE("Reverse document iterators can be pre-incremented across rows.", "ReverseDocumentIterator") {
  Document document("A\nB");
  ReverseDocumentIterator cursor = document.rbegin();
  ++cursor;
  ++cursor;

  REQUIRE(*cursor  == 'A');
  REQUIRE(cursor.location() == Location(0, 0));
}

TEST_CASE("Reverse document iterators can be post-incremented.", "ReverseDocumentIterator") {
  Document document("Quip!");
  ReverseDocumentIterator cursor = document.rbegin();
  ReverseDocumentIterator result = cursor++;
  
  REQUIRE(cursor == document.rbegin());
  REQUIRE(*result == 'p');
}

TEST_CASE("Reverse document iterators can be pre-decremented.", "ReverseDocumentIterator") {
  Document document("Quip!");
  ReverseDocumentIterator cursor = document.rend();
  --cursor;
  
  REQUIRE(*cursor == 'Q');
}

TEST_CASE("Reverse document iterators can be pre-decremented across rows.", "ReverseDocumentIterator") {
  Document document("A\nB");
  ReverseDocumentIterator cursor = document.rend();
  --cursor;
  --cursor;
  
  REQUIRE(*cursor == '\n');
}

TEST_CASE("Reverse document iterators can be post-decremented.", "ReverseDocumentIterator") {
  Document document("Quip!");
  ReverseDocumentIterator cursor = document.rend();
  ReverseDocumentIterator result = cursor--;
  
  REQUIRE(cursor == document.rend());
  REQUIRE(*result == 'Q');
}

TEST_CASE("Reverse document iterators can be compared with ==.", "ReverseDocumentIterator") {
  Document document("Quip!");
  ReverseDocumentIterator a = document.rbegin();
  ReverseDocumentIterator b = document.rbegin();
  
  REQUIRE(a == b);
}

TEST_CASE("Reverse document iterators can be compared with !=.", "ReverseDocumentIterator") {
  Document document("Quip!");
  ReverseDocumentIterator a = document.rbegin();
  ReverseDocumentIterator b = document.rbegin();
  ++b;
  
  REQUIRE(a != b);
}
