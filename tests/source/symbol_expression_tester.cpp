#include <catch.hpp>
#include "viper/viper.hpp"

using namespace viper;

TEST_CASE("test_symbol_expression", "[symbol_expression]") {
  symbol_expression s("abc");
  std::string query;
  s.append_query(query);
  REQUIRE(query == "abc");
}

TEST_CASE("test_sym", "[symbol_expression]") {
  auto s = sym("bca");
  std::string query;
  s.append_query(query);
  REQUIRE(query == "bca");
}
