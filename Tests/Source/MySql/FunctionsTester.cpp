#include <catch.hpp>
#include "Viper/MySql/MySql.hpp"

using namespace Viper;
using namespace Viper::MySql;

TEST_CASE("test_mysql_greatest", "[MySqlFunctions]") {
  auto e = greatest(sym("a"), sym("b"));
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "GREATEST(a, b)");
}

TEST_CASE("test_mysql_least", "[MySqlFunctions]") {
  auto e = least(sym("a"), literal(5));
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "LEAST(a, 5)");
}
