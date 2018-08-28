#ifndef VIPER_SQL_FUNCTIONS_HPP
#define VIPER_SQL_FUNCTIONS_HPP
#include <string>
#include "viper/table.hpp"

namespace Viper {

  //! Returns the maximum value within a column.
  /*!
    \param column The name of the column.
  */
  template<typename T>
  table<T> max(std::string column) {
    return table<T>().add_column("MAX(" + column + ")");
  }

  //! Returns the number of columns matching a query.
  /*!
    \param column The name of the column.
  */
  inline table<int> count(std::string column) {
    return table<int>().add_column("COUNT(" + column + ")");
  }
}

#endif
