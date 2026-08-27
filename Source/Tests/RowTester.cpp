#include <array>
#include <doctest/doctest.h>
#include "Viper/Row.hpp"

using namespace Viper;

namespace {
  struct Point {
    int x;
    int y;
  };

  struct Entry {
    int a;
    Point b;
  };
}

TEST_SUITE("Row") {
  TEST_CASE("single_column_row") {
    auto row = Row<int>().add_column("x");
  }

  TEST_CASE("extending_row") {
    auto r1 = Row<Point>().
      add_column("x", &Point::x).
      add_column("y", &Point::y);
    auto r2 = Row<Entry>().
      add_column("a", &Entry::a).
      extend(r1, &Entry::b);
    auto value = Entry();
    std::array<RawColumn, 3> row_values;
    row_values[0].m_data = "123";
    row_values[1].m_data = "456";
    row_values[2].m_data = "789";
    r2.extract(row_values.data(), value);
    REQUIRE(value.a == 123);
    REQUIRE(value.b.x == 456);
    REQUIRE(value.b.y == 789);
  }

  TEST_CASE("lambda_getter_setter_column") {
    auto row = Row<Point>().
      add_column("x",
        [] (auto& row) -> auto& {
          return row.x;
        });
  }
}
