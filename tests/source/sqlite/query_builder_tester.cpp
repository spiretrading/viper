#include <catch.hpp>
#include "viper/sqlite/sqlite.hpp"

using namespace viper;
using namespace viper::sqlite3;

namespace {
  struct Data {
    int m_x;
  };

  auto get_table() {
    return table<Data>().
      add_column("x", &Data::m_x).
      set_primary_key({"x"});
  }
}

TEST_CASE("test_build_select_query", "[sqlite_query_builder]") {
  SECTION("Simple select query.") {
    std::vector<Data> rows;
    auto s = select(get_table(), "t1", std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x FROM t1;");
  }
  SECTION("Select query with a where clause.") {
    std::vector<Data> rows;
    auto s = select(get_table(), "t1", sym("x") > 5, std::back_inserter(rows));
    std::string q;
    build_query(s, q);
    REQUIRE(q == "SELECT x FROM t1 WHERE (x > 5);");
  }
}
