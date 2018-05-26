#ifndef VIPER_COLUMN_HPP
#define VIPER_COLUMN_HPP
#include <memory>
#include <string>
#include <utility>
#include "viper/data_types/data_type.hpp"

namespace viper {

  //! Represents an SQL column.
  struct column {

    //! The name of the column.
    std::string m_name;

    //! The column's data type.
    std::shared_ptr<data_type> m_type;

    //! Constructs a column.
    /*!
      \param name The name of the column.
      \param type The column's data type.
    */
    column(std::string name, const data_type& type);
  };

  inline column::column(std::string name, const data_type& type)
      : m_name(std::move(name)),
        m_type(type.clone()) {}
}

#endif
