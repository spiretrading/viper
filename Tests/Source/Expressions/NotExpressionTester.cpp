#include <catch.hpp>
#include "Viper/Sqlite3/QueryBuilder.hpp"
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_CASE("test_literal_not", "[not_expression]") {
  auto l = NotExpression(literal(true));
  auto query = std::string();
  Sqlite3::build_query(l, query);
  REQUIRE(query == "(NOT 1)");
}
