#include <catch.hpp>
#include "viper/viper.hpp"

using namespace viper;

TEST_CASE("test_literal_expression", "[literal_expression]") {
  literal_expression l("123");
  std::string query;
  l.append_query(query);
  REQUIRE(query == "123");
}
