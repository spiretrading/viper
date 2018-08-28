#include <catch.hpp>
#include "viper/viper.hpp"

using namespace viper;

TEST_CASE("test_infix_symbols", "[infix_operator]") {
  REQUIRE(get_symbol(infix_operator::type::ADD) == "+");
  REQUIRE(get_symbol(infix_operator::type::SUBTRACT) == "-");
  REQUIRE(get_symbol(infix_operator::type::MULTIPLY) == "*");
  REQUIRE(get_symbol(infix_operator::type::DIVIDE) == "/");
  REQUIRE(get_symbol(infix_operator::type::LESS_THAN) == "<");
  REQUIRE(get_symbol(infix_operator::type::LESS_THAN_OR_EQUAL) == "<=");
  REQUIRE(get_symbol(infix_operator::type::EQUAL) == "=");
  REQUIRE(get_symbol(infix_operator::type::NOT_EQUAL) == "<>");
  REQUIRE(get_symbol(infix_operator::type::GREATER_THAN_OR_EQUAL) == ">=");
  REQUIRE(get_symbol(infix_operator::type::GREATER_THAN) == ">");
  REQUIRE(get_symbol(infix_operator::type::AND) == "AND");
  REQUIRE(get_symbol(infix_operator::type::OR) == "OR");
}

TEST_CASE("test_infix_operator", "[infix_operator]") {
  infix_operator o(infix_operator::type::ADD, sym("a"), sym("b"));
  std::string query;
  o.append_query(query);
  REQUIRE(query == "(a + b)");
}

TEST_CASE("test_infix_operator_overloading", "[infix_operator]") {
  SECTION("Overload of two expressions.") {
    auto o = sym("a") + sym("b");
    std::string query;
    o.append_query(query);
    REQUIRE(query == "(a + b)");
  }
  SECTION("Overload of left expression and right literal.") {
    auto o = sym("a") + 5;
    std::string query;
    o.append_query(query);
    REQUIRE(query == "(a + 5)");
  }
  SECTION("Overload of left literal and right expression.") {
    auto o = 123 + sym("b");
    std::string query;
    o.append_query(query);
    REQUIRE(query == "(123 + b)");
  }
  SECTION("Overload of string literal and right expression.") {
    auto o = "hello\nworld" + sym("b");
    std::string query;
    o.append_query(query);
    REQUIRE(query == "(\"hello\\nworld\" + b)");
  }
}
