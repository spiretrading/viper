#include <catch.hpp>
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_CASE("test_infix_symbols", "[InfixOperator]") {
  REQUIRE(get_symbol(InfixOperator::Type::ADD) == "+");
  REQUIRE(get_symbol(InfixOperator::Type::SUBTRACT) == "-");
  REQUIRE(get_symbol(InfixOperator::Type::MULTIPLY) == "*");
  REQUIRE(get_symbol(InfixOperator::Type::DIVIDE) == "/");
  REQUIRE(get_symbol(InfixOperator::Type::LESS_THAN) == "<");
  REQUIRE(get_symbol(InfixOperator::Type::LESS_THAN_OR_EQUAL) == "<=");
  REQUIRE(get_symbol(InfixOperator::Type::EQUAL) == "=");
  REQUIRE(get_symbol(InfixOperator::Type::NOT_EQUAL) == "<>");
  REQUIRE(get_symbol(InfixOperator::Type::GREATER_THAN_OR_EQUAL) == ">=");
  REQUIRE(get_symbol(InfixOperator::Type::GREATER_THAN) == ">");
  REQUIRE(get_symbol(InfixOperator::Type::AND) == "AND");
  REQUIRE(get_symbol(InfixOperator::Type::OR) == "OR");
}

TEST_CASE("test_InfixOperator", "[InfixOperator]") {
  InfixOperator o(InfixOperator::Type::ADD, sym("a"), sym("b"));
  std::string query;
  o.append_query(query);
  REQUIRE(query == "(a + b)");
}

TEST_CASE("test_InfixOperator_overloading", "[InfixOperator]") {
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
