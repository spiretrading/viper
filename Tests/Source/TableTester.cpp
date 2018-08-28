#include <catch.hpp>
#include "Viper/Table.hpp"

using namespace Viper;

TEST_CASE("test_single_column_table", "[table_tester]") {
  auto t = Table<int>().add_column("x");
}
