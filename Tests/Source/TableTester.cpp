#include <catch.hpp>
#include "Viper/Row.hpp"

using namespace Viper;

TEST_CASE("test_single_column_row", "[row_tester]") {
  auto t = Row<int>().add_column("x");
}
