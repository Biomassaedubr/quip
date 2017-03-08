#include "catch.hpp"

#include "SelectionSet.hpp"

using namespace quip;

TEST_CASE("Selection sets can be default-constructed.", "[SelectionSetTests]") {
  SelectionSet empty;
  
  REQUIRE(empty.count() == 0);
}

TEST_CASE("Selection sets can be constructed from a single selection.", "[SelectionSetTests]") {
  Selection selection(Location(0, 0), Location(1, 0));
  SelectionSet set(selection);
  
  REQUIRE(set.count() == 1);
  REQUIRE(set.primary() == selection);
}

TEST_CASE("Selection sets can be constructed from multiple selections.", "[SelectionSetTests]") {
  Selection a(Location(0, 0), Location(1, 0));
  Selection b(Location(0, 1), Location(1, 1));
  std::vector<Selection> selections { a, b };
  SelectionSet set(selections);
  
  REQUIRE(set.count() == 2);
  REQUIRE(set.primary() == a);
}

TEST_CASE("Selection sets can be indexed.", "[SelectionSetTests]") {
  Selection a(Location(0, 0), Location(1, 0));
  Selection b(Location(0, 1), Location(1, 1));
  std::vector<Selection> selections { a, b };
  SelectionSet set(selections);
  
  REQUIRE(set[0] == a);
  REQUIRE(set[1] == b);
}

TEST_CASE("Selection sets can be iterated.", "[SelectionSetTests]") {
  Selection a(Location(0, 0), Location(1, 0));
  Selection b(Location(5, 5), Location(6, 5));
  std::vector<Selection> selections { a, b };
  SelectionSet set(selections);
  
  std::size_t index = 0;
  SelectionSetIterator cursor = set.begin();
  while (cursor != set.end()) {
    REQUIRE(*cursor == selections[index]);
    ++cursor;
    ++index;
  }
}

TEST_CASE("Selection sets are sorted by origin.", "[SelectionSetTests]") {
  Selection a(Location(0, 0), Location(0, 5));
  Selection b(Location(5, 10), Location(0, 8));
  Selection c(Location(10, 10), Location(0, 15));
  std::vector<Selection> selections { c, b, a };
  
  SelectionSet set(selections);
  SelectionSetIterator cursor = set.begin();
  REQUIRE(*cursor == a);
  
  ++cursor;
  REQUIRE(*cursor == b);
  
  ++cursor;
  REQUIRE(*cursor == c);
}

TEST_CASE("Selection sets collapse overlapping selections.", "[SelectionSetTests]") {
  Selection a(Location(1, 0), Location(10, 0));
  Selection b(Location(8, 0), Location(0, 5));
  Selection c(Location(0, 3), Location(0, 7));
  std::vector<Selection> selections { c, b, a };
  
  SelectionSet set(selections);
  REQUIRE(set.count() == 1);
  
  SelectionSetIterator cursor = set.begin();
  REQUIRE(cursor->origin() == Location(1, 0));
  REQUIRE(cursor->extent() == Location(0, 7));
}

TEST_CASE("Selection sets collapse pairs of overlapping selections.", "[SelectionSetTests]") {
  Selection a(Location(1, 0), Location(10, 0));
  Selection b(Location(5, 0), Location(15, 0));
  Selection c(Location(0, 5), Location(15, 5));
  Selection d(Location(10, 5), Location(0, 10));
  std::vector<Selection> selections { d, c, b, a };
  
  SelectionSet set(selections);
  REQUIRE(set.count() == 2);
  
  SelectionSetIterator cursor = set.begin();
  REQUIRE(cursor->origin() == Location(1, 0));
  REQUIRE(cursor->extent() == Location(15, 0));
  
  ++cursor;
  REQUIRE(cursor->origin() == Location(0, 5));
  REQUIRE(cursor->extent() == Location(0, 10));
}
