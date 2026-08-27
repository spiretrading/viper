#include <doctest/doctest.h>
#include "Viper/Sqlite3/QueryBuilder.hpp"
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_SUITE("LiteralExpression") {
  TEST_CASE("literal_expression") {
    auto l = LiteralExpression("123");
    auto query = std::string();
    Sqlite3::build_query(l, query);
    REQUIRE(query == "123");
  }

  TEST_CASE("int_literal_expression") {
    auto l = literal(5);
    auto query = std::string();
    Sqlite3::build_query(l, query);
    REQUIRE(query == "5");
  }

  TEST_CASE("double_literal_expression") {
    auto l = literal(1.5);
    auto query = std::string();
    Sqlite3::build_query(l, query);
    REQUIRE(query == "1.500000");
  }

  TEST_CASE("string_literal_expression") {
    auto l = literal("abc");
    auto query = std::string();
    Sqlite3::build_query(l, query);
    REQUIRE(query == "\"abc\"");
  }

  TEST_CASE("date_literal_expression") {
    auto dateTime = DateTime(2018, 3, 3, 12, 43, 12, 400);
    auto l = literal(dateTime);
    auto query = std::string();
    Sqlite3::build_query(l, query);
    REQUIRE(query == "'2018-03-03 12:43:12.400'");
  }
}
