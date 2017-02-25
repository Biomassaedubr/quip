#include "catch.hpp"

#include "Coordinate.hpp"

using namespace quip;

TEST_CASE("Coordinates can be default-constructed.", "[CoordinateTests]") {
  Coordinate coordinate;
  
  REQUIRE(coordinate.x == Approx(0.0f));
  REQUIRE(coordinate.y == Approx(0.0f));
}

TEST_CASE("Coordinates can be constructed from values.", "[CoordinateTests]") {
  Coordinate coordinate(-10.5f, 91.2f);
  
  REQUIRE(coordinate.x == Approx(-10.5f));
  REQUIRE(coordinate.y == Approx(91.2f));
}
