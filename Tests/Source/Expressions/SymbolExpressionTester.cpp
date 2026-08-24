#include <catch.hpp>
#include "Viper/Sqlite3/QueryBuilder.hpp"
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_CASE("test_symbol_expression", "[symbol_expression]") {
  SymbolExpression s("abc");
  std::string query;
  Sqlite3::build_query(s, query);
  REQUIRE(query == "abc");
}

TEST_CASE("test_sym", "[symbol_expression]") {
  auto s = sym("bca");
  std::string query;
  Sqlite3::build_query(s, query);
  REQUIRE(query == "bca");
}
