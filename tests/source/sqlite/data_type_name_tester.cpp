#include <catch.hpp>
#include "viper/sqlite/sqlite.hpp"

using namespace viper;
using namespace viper::sqlite3;

TEST_CASE("test_get_name", "[sqlite_data_type_names]") {
  REQUIRE(get_name(f32) == "REAL");
  REQUIRE(get_name(f64) == "REAL");
  REQUIRE(get_name(tiny_int) == "INTEGER");
  REQUIRE(get_name(tiny_uint) == "INTEGER");
  REQUIRE(get_name(small_int) == "INTEGER");
  REQUIRE(get_name(small_uint) == "INTEGER");
  REQUIRE(get_name(integer) == "INTEGER");
  REQUIRE(get_name(uinteger) == "INTEGER");
  REQUIRE(get_name(big_int) == "INTEGER");
  REQUIRE(get_name(big_uint) == "INTEGER");
  REQUIRE(get_name(varchar()) == "TEXT");
  REQUIRE(get_name(varchar(100)) == "TEXT");
}
