#include <catch.hpp>
#include "Viper/Sqlite3/QueryBuilder.hpp"
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_CASE("test_no_argument_function", "[FunctionExpression]") {
  auto e = call("NOW", {});
  auto query = std::string();
  Sqlite3::build_query(e, query);
  REQUIRE(query == "NOW()");
}

TEST_CASE("test_single_argument_function", "[FunctionExpression]") {
  auto e = call("ABS", {sym("a")});
  auto query = std::string();
  Sqlite3::build_query(e, query);
  REQUIRE(query == "ABS(a)");
}

TEST_CASE("test_multiple_argument_function", "[FunctionExpression]") {
  auto e = call("COALESCE", {sym("a"), literal(5)});
  auto query = std::string();
  Sqlite3::build_query(e, query);
  REQUIRE(query == "COALESCE(a, 5)");
}

TEST_CASE("test_nested_function", "[FunctionExpression]") {
  auto e = call("ABS", {call("COALESCE", {sym("a"), sym("b")})});
  auto query = std::string();
  Sqlite3::build_query(e, query);
  REQUIRE(query == "ABS(COALESCE(a, b))");
}

TEST_CASE("test_function_of_expression", "[FunctionExpression]") {
  auto e = call("ABS", {sym("a") + literal(1)});
  auto query = std::string();
  Sqlite3::build_query(e, query);
  REQUIRE(query == "ABS((a + 1))");
}
