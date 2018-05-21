#ifndef VIPER_COLUMN_HPP
#define VIPER_COLUMN_HPP
#include <string>
#include "viper/sql_data_type.hpp"

namespace viper {

  //! Represents an SQL column.
  struct column {

    //! The name of the column.
    std::string m_name;

    //! The column's data type.
    sql_data_type m_type;
  };
}

#endif
