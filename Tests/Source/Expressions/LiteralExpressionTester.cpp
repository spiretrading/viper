#include <catch.hpp>
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_CASE("test_literal_expression", "[literal_expression]") {
  auto l = LiteralExpression("123");
  auto query = std::string();
  l.append_query(query);
  REQUIRE(query == "123");
}

TEST_CASE("test_int_literal_expression", "[literal_expression]") {
  auto l = literal(5);
  auto query = std::string();
  l.append_query(query);
  REQUIRE(query == "5");
}

TEST_CASE("test_double_literal_expression", "[literal_expression]") {
  auto l = literal(1.5);
  auto query = std::string();
  l.append_query(query);
  REQUIRE(query == "1.500000");
}

TEST_CASE("test_string_literal_expression", "[literal_expression]") {
  auto l = literal("abc");
  auto query = std::string();
  l.append_query(query);
  REQUIRE(query == "\"abc\"");
}

TEST_CASE("test_date_literal_expression", "[literal_expression]") {
  auto dateTime = DateTime(2018, 5, 3, 12, 43, 12, 400);
  auto l = literal(dateTime);
  auto query = std::string();
  l.append_query(query);
  REQUIRE(query == "'2018-06-03 12:43:12.400'");
}
