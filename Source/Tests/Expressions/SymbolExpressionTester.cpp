#include <doctest/doctest.h>
#include "Viper/Sqlite3/QueryBuilder.hpp"
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_SUITE("SymbolExpression") {
  TEST_CASE("sym") {
    auto s = sym("bca");
    std::string query;
    Sqlite3::build_query(s, query);
    REQUIRE(query == "bca");
  }
}
