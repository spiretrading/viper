#ifndef VIPER_CREATE_TABLE_STATEMENT_HPP
#define VIPER_CREATE_TABLE_STATEMENT_HPP
#include <string>
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
        \param exists_flag Only create the table if it doesn't exist.
      */
      create_table_statement(table t, std::string name, bool exists_flag);

      //! Returns the table to create.
      const table& get_table() const;

      //! Returns the name of the table.
      const std::string& get_name() const;

      //! Whether the table should be created only if it doesn't exist yet.
      bool get_exists_flag() const;

    private:
      table m_table;
      std::string m_name;
      bool m_exists_flag;
  };

  //! Builds a create table statement.
  /*!
    \param t The table to create.
    \param name The name of the table.
  */
  template<typename T>
  auto create(table<T> t, std::string name) {
    return create_table_statement(std::move(t), std::move(name), false);
  }

  //! Builds a create table statement if it doesn't already exist.
  /*!
    \param t The table to create.
    \param name The name of the table.
  */
  template<typename T>
  auto create_if_not_exists(table<T> t, std::string name) {
    return create_table_statement(std::move(t), std::move(name), true);
  }

  template<typename T>
  create_table_statement<T>::create_table_statement(table t, std::string name,
      bool exists_flag)
      : m_table(std::move(t)),
        m_name(std::move(name)),
        m_exists_flag(exists_flag) {}

  template<typename T>
  const typename create_table_statement<T>::table&
      create_table_statement<T>::get_table() const {
    return m_table;
  }

  template<typename T>
  const std::string& create_table_statement<T>::get_name() const {
    return m_name;
  }

  template<typename T>
  bool create_table_statement<T>::get_exists_flag() const {
    return m_exists_flag;
  }
}

#endif
