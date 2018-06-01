#include <catch.hpp>
#include "viper/table.hpp"

using namespace viper;

TEST_CASE("test_single_column_table", "[table_tester]") {
  auto t = table<int>().add_column("x");
}
