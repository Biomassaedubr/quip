#include "catch.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"

using namespace quip;

TEST_CASE("Document begin iterator is zero-zero.", "DocumentIterator") {
  Document document("A\nB");
  Document::Iterator cursor = document.begin();
  
  REQUIRE(cursor.location() == Location(0, 0));
}

TEST_CASE("Document end iterator is one-past the last column on the last row.", "DocumentIterator") {
  Document document("A\nB");
  Document::Iterator cursor = document.end();
  REQUIRE(cursor.location() == Location(1, 1));
  
  cursor = document.begin();
  ++cursor;
  ++cursor;
  ++cursor;
  REQUIRE(cursor == document.end());
}

TEST_CASE("Document iterators can be dereferenced.", "DocumentIterator") {
  Document document("Q");
  Document::Iterator cursor = document.begin();
  
  REQUIRE(*cursor == 'Q');
  REQUIRE(cursor.location() == Location(0, 0));
}

TEST_CASE("Document iterators can be pre-incremented.", "DocumentIterator") {
  Document document("Quip!");
  Document::Iterator cursor = document.begin();
  ++cursor;
  
  REQUIRE(*cursor == 'u');
  REQUIRE(cursor.location() == Location(1, 0));
}

TEST_CASE("Document iterators can be post-incremented.", "DocumentIterator") {
  Document document("Quip!");
  Document::Iterator cursor = document.begin();
  Document::Iterator result = cursor++;
  
  REQUIRE(cursor == document.begin());
  REQUIRE(*result == 'u');
}

TEST_CASE("Document iterators can be pre-incremented across rows.", "DocumentIterator") {
  Document document("A\nB");
  Document::Iterator cursor = document.begin();
  ++cursor;
  ++cursor;
  
  REQUIRE(*cursor  == 'B');
  REQUIRE(cursor.location() == Location(0, 1));
}

TEST_CASE("Document iterators can be pre-decremented.", "DocumentIterator") {
  Document document("Quip!");
  Document::Iterator cursor = document.end();
  --cursor;
  
  REQUIRE(*cursor == '!');
}

TEST_CASE("Document iterators can be post-decremented.", "DocumentIterator") {
  Document document("Quip!");
  Document::Iterator cursor = document.end();
  Document::Iterator result = cursor--;
  
  REQUIRE(cursor == document.end());
  REQUIRE(*result == '!');
}

TEST_CASE("Document iterators can be pre-decremented across rows.", "DocumentIterator") {
  Document document("A\nB");
  Document::Iterator cursor = document.end();
  --cursor;
  --cursor;
  
  REQUIRE(*cursor == '\n');
}

TEST_CASE("Document iterators can advance by one row.", "DocumentIterator") {
  Document document("ABC\nXYZ");
  Document::Iterator cursor = document.from(Location(1, 0));
  cursor.advanceByRows(1);
  
  REQUIRE(*cursor == 'Y');
}

TEST_CASE("Document iterators can advance by one row to a shorter row.", "DocumentIterator") {
  Document document("123456789\nXYZ");
  Document::Iterator cursor = document.from(Location(6, 0));
  cursor.advanceByRows(1);
  
  REQUIRE(*cursor == 'Z');
}

TEST_CASE("Document iterators can advance by many rows.", "DocumentIterator") {
  Document document("123\n456\n789\nABC");
  Document::Iterator cursor = document.from(Location(1, 0));
  cursor.advanceByRows(3);
  
  REQUIRE(*cursor == 'B');
}

TEST_CASE("Document iterators can advance by one row from the last row.", "DocumentIterator") {
  Document document("ABCDEFG\n1234567");
  Document::Iterator cursor = document.from(Location(2, 1));
  cursor.advanceByRows(1);
  
  REQUIRE(cursor == document.end());
}

TEST_CASE("Document iterators can reverse by one row.", "DocumentIterator") {
  Document document("ABC\nXYZ");
  Document::Iterator cursor = document.from(Location(1, 1));
  cursor.reverseByRows(1);
  
  REQUIRE(*cursor == 'B');
}

TEST_CASE("Document iterators can reverse by one row to a shorter row.", "DocumentIterator") {
  Document document("XYZ\n123456789");
  Document::Iterator cursor = document.from(Location(6, 1));
  cursor.reverseByRows(1);
  
  REQUIRE(*cursor == '\n');
}

TEST_CASE("Document iterators can reverse by many rows.", "DocumentIterator") {
  Document document("123\n456\n789\nABC");
  Document::Iterator cursor = document.from(Location(1, 3));
  cursor.reverseByRows(3);
  
  REQUIRE(*cursor == '2');
}

TEST_CASE("Document iterators can reverse by one row from the first row.", "DocumentIterator") {
  Document document("ABCDEFG\n1234567");
  Document::Iterator cursor = document.from(Location(2, 0));
  cursor.reverseByRows(1);
  
  REQUIRE(cursor == document.begin());
}

TEST_CASE("Document iterators can be compared with ==.", "DocumentIterator") {
  Document document("Quip!");
  Document::Iterator a = document.begin();
  Document::Iterator b = document.begin();
  
  REQUIRE(a == b);
}

TEST_CASE("Document iterators can be compared with !=.", "DocumentIterator") {
  Document document("Quip!");
  Document::Iterator a = document.begin();
  Document::Iterator b = document.begin();
  ++b;
  
  REQUIRE(a != b);
}
