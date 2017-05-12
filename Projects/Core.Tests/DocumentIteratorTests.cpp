#include "catch.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"

using namespace quip;

TEST_CASE("Document begin iterator is zero-zero.", "DocumentIterator") {
  Document document("A\nB");
  DocumentIterator cursor = document.begin();
  
  REQUIRE(cursor.location() == Location(0, 0));
}

TEST_CASE("Document end iterator is one-past the last column on the last row.", "DocumentIterator") {
  Document document("A\nB");
  DocumentIterator cursor = document.end();
  REQUIRE(cursor.location() == Location(1, 1));
  
  cursor = document.begin();
  ++cursor;
  ++cursor;
  ++cursor;
  REQUIRE(cursor == document.end());
}

TEST_CASE("Document iterators can be dereferenced.", "DocumentIterator") {
  Document document("Q");
  DocumentIterator cursor = document.begin();
  
  REQUIRE(*cursor == 'Q');
  REQUIRE(cursor.location() == Location(0, 0));
}

TEST_CASE("Document iterators can be pre-incremented.", "DocumentIterator") {
  Document document("Quip!");
  DocumentIterator cursor = document.begin();
  ++cursor;
  
  REQUIRE(*cursor == 'u');
  REQUIRE(cursor.location() == Location(1, 0));
}

TEST_CASE("Document iterators can be post-incremented.", "DocumentIterator") {
  Document document("Quip!");
  DocumentIterator cursor = document.begin();
  DocumentIterator result = cursor++;
  
  REQUIRE(cursor == document.begin());
  REQUIRE(*result == 'u');
}

TEST_CASE("Document iterators can be pre-incremented across rows.", "DocumentIterator") {
  Document document("A\nB");
  DocumentIterator cursor = document.begin();
  ++cursor;
  ++cursor;
  
  REQUIRE(*cursor  == 'B');
  REQUIRE(cursor.location() == Location(0, 1));
}

TEST_CASE("Document iterators can be pre-decremented.", "DocumentIterator") {
  Document document("Quip!");
  DocumentIterator cursor = document.end();
  --cursor;
  
  REQUIRE(*cursor == '!');
}

TEST_CASE("Document iterators can be post-decremented.", "DocumentIterator") {
  Document document("Quip!");
  DocumentIterator cursor = document.end();
  DocumentIterator result = cursor--;
  
  REQUIRE(cursor == document.end());
  REQUIRE(*result == '!');
}

TEST_CASE("Document iterators can be pre-decremented across rows.", "DocumentIterator") {
  Document document("A\nB");
  DocumentIterator cursor = document.end();
  --cursor;
  --cursor;
  
  REQUIRE(*cursor == '\n');
}

TEST_CASE("Document iterators can be compared with ==.", "DocumentIterator") {
  Document document("Quip!");
  DocumentIterator a = document.begin();
  DocumentIterator b = document.begin();
  
  REQUIRE(a == b);
}

TEST_CASE("Document iterators can be compared with !=.", "DocumentIterator") {
  Document document("Quip!");
  DocumentIterator a = document.begin();
  DocumentIterator b = document.begin();
  ++b;
  
  REQUIRE(a != b);
}
