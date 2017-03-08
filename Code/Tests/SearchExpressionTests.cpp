#include "catch.hpp"

#include "SearchExpression.hpp"

using namespace quip;


TEST_CASE("Search expressions can be constructed from an empty expression.", "[SearchExpressionTests]") {
  SearchExpression expression("");
  
  REQUIRE_FALSE(expression.valid());
}

TEST_CASE("Search expressions can be constructed from a simple expression.", "[SearchExpressionTests]") {
  SearchExpression expression("foo");
  
  REQUIRE(expression.valid());
  REQUIRE(expression.expression() == "foo");
}

TEST_CASE("Search expressions can be constructed from an expression with a trailing class.", "[SearchExpressionTests]") {
  SearchExpression expression("[a-z");
  
  REQUIRE_FALSE(expression.valid());
}

TEST_CASE("Search expressions can be constructed from an expression with a trailing slash.", "[SearchExpressionTests]") {
  SearchExpression expression("\\");
  
  REQUIRE_FALSE(expression.valid());
}
