#include <catch.hpp>
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_CASE("test_no_argument_function", "[FunctionExpression]") {
  auto e = call("NOW", {});
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "NOW()");
}

TEST_CASE("test_single_argument_function", "[FunctionExpression]") {
  auto e = call("ABS", {sym("a")});
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "ABS(a)");
}

TEST_CASE("test_multiple_argument_function", "[FunctionExpression]") {
  auto e = call("GREATEST", {sym("a"), literal(5)});
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "GREATEST(a, 5)");
}

TEST_CASE("test_nested_function", "[FunctionExpression]") {
  auto e = call("ABS", {call("LEAST", {sym("a"), sym("b")})});
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "ABS(LEAST(a, b))");
}

TEST_CASE("test_function_of_expression", "[FunctionExpression]") {
  auto e = call("ABS", {sym("a") + literal(1)});
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "ABS((a + 1))");
}
