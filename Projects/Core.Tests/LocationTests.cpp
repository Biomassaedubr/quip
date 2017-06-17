#include "catch.hpp"

#include "Location.hpp"

using namespace quip;

TEST_CASE("Locations can be default-constructed.", "Location") {
  Location location;
  
  REQUIRE(location.isValid());
  REQUIRE(location.column() == 0);
  REQUIRE(location.row() == 0);
}

TEST_CASE("Locations can be constructed from values.", "Location") {
  Location location(1, 10);
  
  REQUIRE(location.isValid());
  REQUIRE(location.column() == 1);
  REQUIRE(location.row() == 10);
}

TEST_CASE("Locations can be adjusted.", "Location") {
  Location location(1, 2);
  Location result = location.adjustBy(10, 20);
  
  REQUIRE(result.isValid());
  REQUIRE(result.column() == 11);
  REQUIRE(result.row() == 22);
}

TEST_CASE("Locations clamp negative adjustments to zero-zero.", "Location") {
  Location location(1, 2);
  Location result = location.adjustBy(-100, -200);
  
  REQUIRE(result.isValid());
  REQUIRE(result.column() == 0);
  REQUIRE(result.row() == 0);
}

TEST_CASE("Create an invalid location.", "Location") {
  Location location = Location::invalid();
  
  REQUIRE(!location.isValid());
}

TEST_CASE("Locations can be compared with ==.", "Location") {
  Location a(1, 2);
  Location b(1, 2);
  
  REQUIRE(a == b);
}

TEST_CASE("Locations can be compared with !=.", "Location") {
  Location a(1, 2);
  Location b(5, 6);
  
  REQUIRE(a != b);
}

TEST_CASE("Locations can be swapped.", "Location") {
  Location a(1, 2);
  Location b(3, 4);
  
  using std::swap;
  swap(a, b);
  
  REQUIRE(a.column() == 3);
  REQUIRE(b.column() == 1);
  REQUIRE(a.row() == 4);
  REQUIRE(b.row() == 2);
}
