#include "catch.hpp"

#include "Extent.hpp"

using namespace quip;

TEST_CASE("Default-construct an extent.", "Extent") {
  Extent extent;
  
  REQUIRE(extent.width() == Approx(0.0f));
  REQUIRE(extent.height() == Approx(0.0f));
}

TEST_CASE("Construct an extent from width and height.", "Extent") {
  Extent extent(1.4f, 2.7f);
  
  REQUIRE(extent.width() == Approx(1.4f));
  REQUIRE(extent.height() == Approx(2.7f));
}
