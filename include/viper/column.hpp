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

    //! Whether the column is nullable.
    bool m_is_nullable;

    //! Constructs a column.
    /*!
      \param name The name of the column.
      \param type The column's data type.
      \param is_nullable Whether the column is nullable.
    */
    column(std::string name, const data_type& type, bool is_nullable);
  };

  inline column::column(std::string name, const data_type& type,
      bool is_nullable)
      : m_name(std::move(name)),
        m_type(type.clone()),
        m_is_nullable(is_nullable) {}
}

#endif
