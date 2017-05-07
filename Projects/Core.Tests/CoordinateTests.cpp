#include "catch.hpp"

#include "Coordinate.hpp"

using namespace quip;

TEST_CASE("Default-construct a coordinate.", "Coordinate") {
  Coordinate coordinate;
  
  REQUIRE(coordinate.x == Approx(0.0f));
  REQUIRE(coordinate.y == Approx(0.0f));
}

TEST_CASE("Construct a coordinate from X and Y.", "Coordinate") {
  Coordinate coordinate(-10.5f, 91.2f);
  
  REQUIRE(coordinate.x == Approx(-10.5f));
  REQUIRE(coordinate.y == Approx(91.2f));
}
