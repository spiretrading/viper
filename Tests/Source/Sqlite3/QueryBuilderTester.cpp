#include <catch.hpp>
#include "Viper/Sqlite3/Sqlite3.hpp"

using namespace Viper;
using namespace Viper::Sqlite3;

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

TEST_CASE("test_build_create_table_query", "[sqlite3_query_builder]") {
  auto s = create(get_row(), "t1");
  std::string q;
  build_query(s, q);
  REQUIRE(q ==
    "BEGIN;"
    "CREATE TABLE t1(x INTEGER NOT NULL,y REAL NOT NULL,PRIMARY KEY(x));"
    "COMMIT;");
}

TEST_CASE("test_build_select_query", "[sqlite3_query_builder]") {
  SECTION("Simple select query.") {
    std::vector<TableRow> rows;
    auto s = select(get_row(), "t1", std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1;");
  }
  SECTION("Select query with a where clause.") {
    std::vector<TableRow> rows;
    auto s = select(get_row(), "t1", sym("x") > 5, std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1 WHERE (x > 5);");
  }
  SECTION("Select query with a limit.") {
    std::vector<TableRow> rows;
    auto s = select(get_row(), "t1", limit(1000), std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1 LIMIT 1000;");
  }
  SECTION("Select query with an ordering.") {
    std::vector<TableRow> rows;
    auto s = select(get_row(), "t1", order_by("x", Order::ASC),
      std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1 ORDER BY x ASC;");
  }
  SECTION("Select query with a multi column ordering.") {
    std::vector<TableRow> rows;
    auto s = select(get_row(), "t1", order_by({"y", "x"}, Order::DESC),
      std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1 ORDER BY (y,x) DESC;");
  }
  SECTION("Select query with a where clause and a limit.") {
    std::vector<TableRow> rows;
    auto s = select(get_row(), "t1", 5 == sym("x"), limit(432),
      std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1 WHERE (5 = x) LIMIT 432;");
  }
  SECTION("Select query with a where, limit, and order clause.") {
    std::vector<TableRow> rows;
    auto s = select(get_row(), "t1", 5 == sym("x"), limit(432),
      order_by("x", Order::DESC), std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1 WHERE (5 = x) ORDER BY x DESC LIMIT 432;");
  }
  SECTION("Select query with an expression column.") {
    int value;
    auto s = select(max<int>("abc"), "t1", &value);
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT MAX(abc) FROM t1;");
  }
}

TEST_CASE("test_recursive_select", "[sqlite3_query_builder]") {
  std::vector<TableRow> rows;
  auto s = select(get_row(), select({"a", "b", "c"}, "t1"),
    std::back_inserter(rows));
  std::string q;
  build_query(s, q);
  REQUIRE(q == "SELECT x,y FROM (SELECT a,b,c FROM t1) AS alias;");
}

TEST_CASE("test_blob", "[sqlite3_query_builder]") {
  auto row = Row<Blob>().add_column("data");
  Connection c(":memory:");
  c.open();
  c.execute(create(row, "test_table"));
  Blob blob;
  char value[13];
  std::memcpy(value, "he\0llo wor\0ld", sizeof(value));
  blob.m_size = sizeof(value);
  blob.m_data = reinterpret_cast<std::byte*>(value);
  c.execute(insert(row, "test_table", &blob));
  Blob select_blob;
  c.execute(select(row, "test_table", &select_blob));
  auto select_data = reinterpret_cast<const char*>(select_blob.m_data);
  REQUIRE(std::memcmp(blob.m_data, select_blob.m_data, blob.m_size) == 0);
}
