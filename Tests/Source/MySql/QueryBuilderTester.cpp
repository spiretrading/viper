#include <catch.hpp>
#include "Viper/MySql/MySql.hpp"

using namespace Viper;
using namespace Viper::MySql;

namespace {
  struct TableRow {
    int m_x;
    double m_y;
  };

  auto get_row() {
    return Row<TableRow>().
      add_column("x", &TableRow::m_x).
      set_primary_key("x").
      add_column("y", &TableRow::m_y);
  }
}

TEST_CASE("test_build_upsert_query", "[mysql_query_builder]") {
  auto row = TableRow{123, 3.14};
  auto s = upsert(get_row(), "t1", &row);
  std::string q;
  build_query(s, q);
  REQUIRE(q == "INSERT INTO t1 (x,y) "
               "VALUES (123,3.140000) "
               "ON DUPLICATE KEY UPDATE y = VALUES(y);");
}

TEST_CASE("test_build_greatest", "[MySqlQueryBuilder]") {
  auto query = std::string();
  build_query(greatest(sym("a"), literal(5)), query);
  REQUIRE(query == "GREATEST(a, 5)");
}

TEST_CASE("test_build_least", "[MySqlQueryBuilder]") {
  auto query = std::string();
  build_query(least(sym("a"), sym("b")), query);
  REQUIRE(query == "LEAST(a, b)");
}

TEST_CASE("test_build_nested_function", "[MySqlQueryBuilder]") {
  auto query = std::string();
  build_query(greatest(greatest(sym("a"), sym("b")), sym("c")), query);
  REQUIRE(query == "GREATEST(GREATEST(a, b), c)");
}

TEST_CASE("test_build_custom_function", "[MySqlQueryBuilder]") {
  auto query = std::string();
  build_query(call("MY_FUNCTION", {sym("a"), literal(1)}), query);
  REQUIRE(query == "MY_FUNCTION(a, 1)");
}
