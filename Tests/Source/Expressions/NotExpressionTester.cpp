#include <catch.hpp>
#include "Viper/Viper.hpp"

using namespace Viper;

TEST_CASE("test_literal_not", "[not_expression]") {
  auto l = NotExpression(literal(true));
  auto query = std::string();
  l.append_query(query);
  REQUIRE(query == "(NOT 1)");
}
