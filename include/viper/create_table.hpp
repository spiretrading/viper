#ifndef VIPER_CREATE_TABLE_STATEMENT_HPP
#define VIPER_CREATE_TABLE_STATEMENT_HPP
#include <string_view>
#include "viper/table.hpp"

namespace viper {

  /*! \brief Represents an SQL CREATE TABLE statement.
      \tparam T The type of table to create.
   */
  template<typename T>
  class create_table_statement {
    public:

      //! The type of table to create.
      using table = T;

      //! Constructs a create table statement.
      /*!
        \param t The table to create.
        \param name The name of the table.
      */
      create_table_statement(table t, std::string_view name);

      //! Returns the table to create.
      const table& get_table() const;

      //! Returns the name of the table.
      const std::string& get_name() const;

    private:
      table m_table;
      std::string m_name;
  };

  //! Builds a create table statement.
  /*!
    \param t The table to create.
    \param name The name of the table.
  */
  template<typename T>
  auto create(table<T> t, std::string_view name) {
    return create_table_statement(std::move(t), name);
  }

  template<typename T>
  create_table_statement<T>::create_table_statement(table t,
      std::string_view name)
      : m_table(std::move(t)),
        m_name(name) {}

  template<typename T>
  const typename create_table_statement<T>::table&
      create_table_statement<T>::get_table() const {
    return m_table;
  }

  template<typename T>
  const std::string& create_table_statement<T>::get_name() const {
    return m_name;
  }
}

#endif
