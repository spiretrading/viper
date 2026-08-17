#include <vector>
#include <catch.hpp>
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_CASE("test_single_value_in", "[in_expression]") {
  auto e = in(sym("a"), {literal(1)});
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "(a IN (1))");
}

TEST_CASE("test_multiple_value_in", "[in_expression]") {
  auto e = in(sym("a"), {literal(1), literal(2), literal(3)});
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "(a IN (1, 2, 3))");
}

TEST_CASE("test_empty_in", "[in_expression]") {
  auto e = in(sym("a"), {});
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "0");
}

TEST_CASE("test_iterator_in", "[in_expression]") {
  auto values = std::vector<int>{4, 5, 6};
  auto e = in(sym("a"), values.begin(), values.end());
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "(a IN (4, 5, 6))");
}

TEST_CASE("test_string_in", "[in_expression]") {
  auto values = std::vector<std::string>{"x", "y"};
  auto e = in(sym("a"), values.begin(), values.end());
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "(a IN (\"x\", \"y\"))");
}
