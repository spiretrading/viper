#include <catch.hpp>
#include "viper/sqlite/sqlite.hpp"

using namespace viper;
using namespace viper::sqlite3;

namespace {
  struct table_row {
    int m_x;
    double m_y;
  };

  auto get_table() {
    return table<table_row>().
      add_column("x", &table_row::m_x).
      set_primary_key({"x"}).
      add_column("y", &table_row::m_y);
  }
}

TEST_CASE("test_build_create_table_query", "[sqlite_query_builder]") {
  auto s = create(get_table(), "t1");
  std::string q;
  build_query(s, q);
  REQUIRE(q ==
    "BEGIN;"
    "CREATE TABLE t1(x INTEGER NOT NULL,y REAL NOT NULL,PRIMARY KEY(x));"
    "COMMIT;");
}

TEST_CASE("test_build_select_query", "[sqlite_query_builder]") {
  SECTION("Simple select query.") {
    std::vector<table_row> rows;
    auto s = select(get_table(), "t1", std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1;");
  }
  SECTION("Select query with a where clause.") {
    std::vector<table_row> rows;
    auto s = select(get_table(), "t1", sym("x") > 5, std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1 WHERE (x > 5);");
  }
  SECTION("Select query with a limit.") {
    std::vector<table_row> rows;
    auto s = select(get_table(), "t1", limit(1000), std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1 LIMIT 1000;");
  }
  SECTION("Select query with an ordering.") {
    std::vector<table_row> rows;
    auto s = select(get_table(), "t1", order_by("x", order::ASC),
      std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1 ORDER BY x ASC;");
  }
  SECTION("Select query with a multi column ordering.") {
    std::vector<table_row> rows;
    auto s = select(get_table(), "t1", order_by({"y", "x"}, order::DESC),
      std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1 ORDER BY (y,x) DESC;");
  }
  SECTION("Select query with a where clause and a limit.") {
    std::vector<table_row> rows;
    auto s = select(get_table(), "t1", 5 == sym("x"), limit(432),
      std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM t1 WHERE (5 = x) LIMIT 432;");
  }
  SECTION("Select query with a where, limit, and order clause.") {
    std::vector<table_row> rows;
    auto s = select(get_table(), "t1", 5 == sym("x"), limit(432),
      order_by("x", order::DESC), std::back_inserter(rows));
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

TEST_CASE("test_recursive_select", "[sqlite_query_builder]") {
  std::vector<table_row> rows;
  auto s = select(get_table(), select({"a", "b", "c"}, "t1"),
    std::back_inserter(rows));
  std::string q;
  build_query(s, q);
  REQUIRE(q == "SELECT x,y FROM (SELECT a,b,c FROM t1) AS alias;");
}
