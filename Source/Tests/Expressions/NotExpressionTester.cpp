#include <doctest/doctest.h>
#include "Viper/Sqlite3/QueryBuilder.hpp"
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_SUITE("NotExpression") {
  TEST_CASE("literal_not") {
    auto l = NotExpression(literal(true));
    auto query = std::string();
    Sqlite3::build_query(l, query);
    REQUIRE(query == "(NOT 1)");
  }
}
