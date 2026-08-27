#include <cstddef>
#include <cstring>
#include <iterator>
#include <string>
#include <tuple>
#include <vector>
#include <doctest/doctest.h>
#include "Viper/Sqlite3/Sqlite3.hpp"

using namespace Viper;
using namespace Viper::Sqlite3;

namespace {
  struct TableRow {
    int m_x;
    double m_y;

    bool operator ==(const TableRow& rhs) const {
      return std::tie(m_x, m_y) == std::tie(rhs.m_x, rhs.m_y);
    }
  };

  auto get_row() {
    return Row<TableRow>().
      add_column("x", &TableRow::m_x).
      set_primary_key("x").
      add_column("y", &TableRow::m_y);
  }
}

TEST_SUITE("Sqlite3QueryBuilder") {
  TEST_CASE("build_create_table_query") {
    auto s = create(get_row(), "t1");
    std::string q;
    build_query(s, q);
    REQUIRE(q ==
      "CREATE TABLE t1(x INTEGER NOT NULL,y REAL NOT NULL,PRIMARY KEY(x));");
  }

  TEST_CASE("build_select_query") {
    SUBCASE("Simple select query.") {
      std::vector<TableRow> rows;
      auto s = select(get_row(), "t1", std::back_inserter(rows));
      std::string q;
      build_query(s, q);
      REQUIRE(q == "SELECT x,y FROM t1;");
    }
    SUBCASE("Select query with a where clause.") {
      std::vector<TableRow> rows;
      auto s = select(get_row(), "t1", sym("x") > 5, std::back_inserter(rows));
      std::string q;
      build_query(s, q);
      REQUIRE(q == "SELECT x,y FROM t1 WHERE (x > 5);");
    }
    SUBCASE("Select query with a limit.") {
      std::vector<TableRow> rows;
      auto s = select(get_row(), "t1", limit(1000), std::back_inserter(rows));
      std::string q;
      build_query(s, q);
      REQUIRE(q == "SELECT x,y FROM t1 LIMIT 1000;");
    }
    SUBCASE("Select query with an ordering.") {
      std::vector<TableRow> rows;
      auto s = select(get_row(), "t1", order_by("x", Order::ASC),
        std::back_inserter(rows));
      std::string q;
      build_query(s, q);
      REQUIRE(q == "SELECT x,y FROM t1 ORDER BY x ASC;");
    }
    SUBCASE("Select query with a multi column ordering.") {
      std::vector<TableRow> rows;
      auto s = select(get_row(), "t1",
        order_by({{"y", Order::DESC}, {"x", Order::DESC}}),
        std::back_inserter(rows));
      std::string q;
      build_query(s, q);
      REQUIRE(q == "SELECT x,y FROM t1 ORDER BY y DESC,x DESC;");
    }
    SUBCASE("Select query with a where clause and a limit.") {
      std::vector<TableRow> rows;
      auto s = select(get_row(), "t1", 5 == sym("x"), limit(432),
        std::back_inserter(rows));
      std::string q;
      build_query(s, q);
      REQUIRE(q == "SELECT x,y FROM t1 WHERE (5 = x) LIMIT 432;");
    }
    SUBCASE("Select query with a where, limit, and order clause.") {
      std::vector<TableRow> rows;
      auto s = select(get_row(), "t1", 5 == sym("x"), limit(432),
        order_by("x", Order::DESC), std::back_inserter(rows));
      std::string q;
      build_query(s, q);
      REQUIRE(q ==
        "SELECT x,y FROM t1 WHERE (5 = x) ORDER BY x DESC LIMIT 432;");
    }
    SUBCASE("Select query with a limit and an offset.") {
      std::vector<TableRow> rows;
      auto s = select(get_row(), "t1", limit(25, 100),
        std::back_inserter(rows));
      std::string q;
      build_query(s, q);
      REQUIRE(q == "SELECT x,y FROM t1 LIMIT 25 OFFSET 100;");
    }
    SUBCASE("Select query with a limit and a zero offset.") {
      std::vector<TableRow> rows;
      auto s = select(get_row(), "t1", limit(25, 0), std::back_inserter(rows));
      std::string q;
      build_query(s, q);
      REQUIRE(q == "SELECT x,y FROM t1 LIMIT 25;");
    }
    SUBCASE("Select query with an order, limit, and offset.") {
      std::vector<TableRow> rows;
      auto s = select(get_row(), "t1", order_by("x", Order::DESC),
        limit(10, 20), std::back_inserter(rows));
      std::string q;
      build_query(s, q);
      REQUIRE(q == "SELECT x,y FROM t1 ORDER BY x DESC LIMIT 10 OFFSET 20;");
    }
    SUBCASE("Select query with an expression column.") {
      int value;
      auto s = select(max<int>("abc"), "t1", &value);
      std::string q;
      build_query(s, q);
      REQUIRE(q == "SELECT MAX(abc) FROM t1;");
    }
  }

  TEST_CASE("recursive_select") {
    std::vector<TableRow> rows;
    auto s = select(get_row(), select({"a", "b", "c"}, "t1"),
      std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x,y FROM (SELECT a,b,c FROM t1) AS alias;");
  }

  TEST_CASE("blob") {
    auto row = Row<std::vector<std::byte>>().add_column("data");
    Connection c(":memory:");
    c.open();
    c.execute(create(row, "test_table"));
    std::vector<std::byte> blob(13);
    std::memcpy(blob.data(), "he\0llo wor\0ld", blob.size());
    c.execute(insert(row, "test_table", &blob));
    std::vector<std::byte> select_blob;
    c.execute(select(row, "test_table", &select_blob));
    REQUIRE(select_blob == blob);
  }

  TEST_CASE("date_time") {
    auto row = Row<DateTime>("value");
    auto c = Connection(":memory:");
    c.open();
    c.execute(create(row, "test_table"));
    auto value = DateTime(2012, 5, 9, 2, 6, 14, 221);
    c.execute(insert(row, "test_table", &value));
    auto selected_value = DateTime();
    c.execute(select(row, "test_table", &selected_value));
    REQUIRE(selected_value == value);
  }

  TEST_CASE("upsert") {
    auto c = Connection(":memory:");
    c.open();
    c.execute(create(get_row(), "test_table"));
    auto value = TableRow{5, 3.14};
    c.execute(insert(get_row(), "test_table", &value));
    auto update = TableRow{5, 6.28};
    c.execute(upsert(get_row(), "test_table", &value));
    auto selected_value = TableRow();
    c.execute(select(get_row(), "test_table", &selected_value));
    REQUIRE(selected_value == value);
  }

  TEST_CASE("build_greatest") {
    auto query = std::string();
    build_query(greatest(sym("a"), literal(5)), query);
    REQUIRE(query == "MAX(a, 5)");
  }

  TEST_CASE("build_least") {
    auto query = std::string();
    build_query(least(sym("a"), sym("b")), query);
    REQUIRE(query == "MIN(a, b)");
  }

  TEST_CASE("build_nested_function") {
    auto query = std::string();
    build_query(greatest(greatest(sym("a"), sym("b")), sym("c")), query);
    REQUIRE(query == "MAX(MAX(a, b), c)");
  }

  TEST_CASE("build_custom_function") {
    auto query = std::string();
    build_query(call("MY_FUNCTION", {sym("a"), literal(1)}), query);
    REQUIRE(query == "MY_FUNCTION(a, 1)");
  }
}
