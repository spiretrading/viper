#ifndef VIPER_SQL_FUNCTIONS_HPP
#define VIPER_SQL_FUNCTIONS_HPP
#include <string>
#include "Viper/Table.hpp"

namespace Viper {

  //! Returns the maximum value within a column.
  /*!
    \param column The name of the column.
  */
  template<typename T>
  Table<T> max(std::string column) {
    return Table<T>().add_column("MAX(" + column + ")");
  }

  //! Returns the number of columns matching a query.
  /*!
    \param column The name of the column.
  */
  inline Table<int> count(std::string column) {
    return Table<int>().add_column("COUNT(" + column + ")");
  }
}

#endif
