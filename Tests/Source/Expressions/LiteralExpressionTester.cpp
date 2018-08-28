#include <catch.hpp>
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_CASE("test_literal_expression", "[literal_expression]") {
  LiteralExpression l("123");
  std::string query;
  l.append_query(query);
  REQUIRE(query == "123");
}

TEST_CASE("test_int_literal_expression", "[literal_expression]") {
  auto l = literal(5);
  std::string query;
  l.append_query(query);
  REQUIRE(query == "5");
}

TEST_CASE("test_double_literal_expression", "[literal_expression]") {
  auto l = literal(1.5);
  std::string query;
  l.append_query(query);
  REQUIRE(query == "1.500000");
}

TEST_CASE("test_string_literal_expression", "[literal_expression]") {
  auto l = literal("abc");
  std::string query;
  l.append_query(query);
  REQUIRE(query == "\"abc\"");
}
