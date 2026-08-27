#include <doctest/doctest.h>
#include "Viper/Sqlite3/QueryBuilder.hpp"
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_SUITE("InfixOperator") {
  TEST_CASE("infix_symbols") {
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

  TEST_CASE("infix_operator_overloading") {
    SUBCASE("Overload of two expressions.") {
      auto o = sym("a") + sym("b");
      std::string query;
      Sqlite3::build_query(o, query);
      REQUIRE(query == "(a + b)");
    }
    SUBCASE("Overload of left expression and right literal.") {
      auto o = sym("a") + 5;
      std::string query;
      Sqlite3::build_query(o, query);
      REQUIRE(query == "(a + 5)");
    }
    SUBCASE("Overload of left literal and right expression.") {
      auto o = 123 + sym("b");
      std::string query;
      Sqlite3::build_query(o, query);
      REQUIRE(query == "(123 + b)");
    }
    SUBCASE("Overload of string literal and right expression.") {
      auto o = "hello\nworld" + sym("b");
      std::string query;
      Sqlite3::build_query(o, query);
      REQUIRE(query == "(\"hello\\nworld\" + b)");
    }
  }
}
