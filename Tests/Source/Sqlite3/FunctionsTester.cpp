#include <catch.hpp>
#include "Viper/Sqlite3/Sqlite3.hpp"

using namespace Viper;
using namespace Viper::Sqlite3;

TEST_CASE("test_sqlite3_greatest", "[Sqlite3Functions]") {
  auto e = greatest(sym("a"), sym("b"));
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "MAX(a, b)");
}

TEST_CASE("test_sqlite3_least", "[Sqlite3Functions]") {
  auto e = least(sym("a"), literal(5));
  auto query = std::string();
  e.append_query(query);
  REQUIRE(query == "MIN(a, 5)");
}
