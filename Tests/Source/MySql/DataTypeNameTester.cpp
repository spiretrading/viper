#include <catch.hpp>
#include "Viper/MySql/DataTypeName.hpp"

using namespace Viper;
using namespace Viper::MySql;

TEST_CASE("test_mysql_get_name", "[mysql_data_type_names]") {
  REQUIRE(get_name(f32) == "FLOAT");
  REQUIRE(get_name(f64) == "DOUBLE");
}
