#ifndef VIPER_COLUMN_HPP
#define VIPER_COLUMN_HPP
#include <memory>
#include <string>
#include <utility>
#include "Viper/DataTypes/DataType.hpp"

namespace Viper {

  //! Represents an SQL column.
  struct Column {

    //! The name of the column.
    std::string m_name;

    //! The column's data type.
    std::shared_ptr<DataType> m_type;

    //! Whether the column is nullable.
    bool m_is_nullable;

    //! Constructs a column.
    /*!
      \param name The name of the column.
      \param type The column's data type.
      \param is_nullable Whether the column is nullable.
    */
    Column(std::string name, const DataType& type, bool is_nullable);
  };

  inline Column::Column(std::string name, const DataType& type,
      bool is_nullable)
      : m_name(std::move(name)),
        m_type(type.clone()),
        m_is_nullable(is_nullable) {}
}

#endif
