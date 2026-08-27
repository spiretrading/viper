#include <doctest/doctest.h>
#include "Viper/Sqlite3/QueryBuilder.hpp"
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_SUITE("FunctionExpression") {
  TEST_CASE("no_argument_function") {
    auto e = call("NOW", {});
    auto query = std::string();
    Sqlite3::build_query(e, query);
    REQUIRE(query == "NOW()");
  }

  TEST_CASE("single_argument_function") {
    auto e = call("ABS", {sym("a")});
    auto query = std::string();
    Sqlite3::build_query(e, query);
    REQUIRE(query == "ABS(a)");
  }

  TEST_CASE("multiple_argument_function") {
    auto e = call("COALESCE", {sym("a"), literal(5)});
    auto query = std::string();
    Sqlite3::build_query(e, query);
    REQUIRE(query == "COALESCE(a, 5)");
  }

  TEST_CASE("nested_function") {
    auto e = call("ABS", {call("COALESCE", {sym("a"), sym("b")})});
    auto query = std::string();
    Sqlite3::build_query(e, query);
    REQUIRE(query == "ABS(COALESCE(a, b))");
  }

  TEST_CASE("function_of_expression") {
    auto e = call("ABS", {sym("a") + literal(1)});
    auto query = std::string();
    Sqlite3::build_query(e, query);
    REQUIRE(query == "ABS((a + 1))");
  }
}
