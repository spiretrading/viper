#include <doctest/doctest.h>
#include "Viper/MySql/DataTypeName.hpp"

using namespace Viper;
using namespace Viper::MySql;

TEST_SUITE("MySqlDataTypeName") {
  TEST_CASE("mysql_get_name") {
    REQUIRE(get_name(f32) == "FLOAT");
    REQUIRE(get_name(f64) == "DOUBLE");
  }
}
