#include "catch.hpp"

#include "Selection.hpp"

using namespace quip;

TEST_CASE("Construct a selection from a single location.", "Selection") {
  Selection selection(Location(1, 2));
  
  REQUIRE(selection.origin() == selection.extent());
  REQUIRE(selection.origin().column() == 1);
  REQUIRE(selection.origin().row() == 2);
}

TEST_CASE("Construct a selection from a column and row pair.", "Selection") {
  Selection selection(1, 2);
  
  REQUIRE(selection.origin() == selection.extent());
  REQUIRE(selection.origin().column() == 1);
  REQUIRE(selection.origin().row() == 2);
}

TEST_CASE("Construct a selection from two locations.", "Selection") {
  Selection selection(Location(1, 2), Location(2, 4));

  REQUIRE(selection.origin().column() == 1);
  REQUIRE(selection.origin().row() == 2);
  REQUIRE(selection.extent().column() == 2);
  REQUIRE(selection.extent().row() == 4);
}

TEST_CASE("Construct a selection from two column and row pairs.", "Selection") {
  Selection selection(1, 2, 2, 4);
  
  REQUIRE(selection.origin().column() == 1);
  REQUIRE(selection.origin().row() == 2);
  REQUIRE(selection.extent().column() == 2);
  REQUIRE(selection.extent().row() == 4);
}

TEST_CASE("Get the height of a single-line selection.", "Selection") {
  Selection selection(1, 2);
  
  REQUIRE(selection.height() == 1);
}

TEST_CASE("Get the height of a multi-line selection.", "Selection") {
  Selection selection(0, 0, 0, 2);
  
  REQUIRE(selection.height() == 3);
}

TEST_CASE("Compare selections with ==.", "Selection") {
  Selection a(Location(0, 0), Location(10, 5));
  Selection b(Location(0, 0), Location(10, 5));
  Selection c(Location(1, 0), Location(1, 5));
  
  REQUIRE(a == b);
  REQUIRE_FALSE(b == c);
}

TEST_CASE("Compare selections with !=.", "Selection") {
  Selection a(Location(0, 0), Location(10, 5));
  Selection b(Location(0, 0), Location(10, 5));
  Selection c(Location(1, 0), Location(1, 5));
  
  REQUIRE_FALSE(a != b);
  REQUIRE(b != c);
}

TEST_CASE("Swap two selections.", "Selection") {
  Selection a(Location(0, 0), Location(1, 1));
  Selection b(Location(2, 2), Location(3, 3));
  
  using std::swap;
  swap(a, b);
  
  REQUIRE(a.origin() == Location(2, 2));
  REQUIRE(b.origin() == Location(0, 0));
  REQUIRE(a.extent() == Location(3, 3));
  REQUIRE(b.extent() == Location(1, 1));
}
