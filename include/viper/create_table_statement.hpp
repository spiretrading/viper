#ifndef VIPER_CREATE_TABLE_STATEMENT_HPP
#define VIPER_CREATE_TABLE_STATEMENT_HPP
#include <string>
#include "viper/table.hpp"

namespace Viper {

  /*! \brief Represents an SQL CREATE TABLE statement.
      \tparam T The type of table to create.
   */
  template<typename T>
  class CreateTableStatement {
    public:

      //! The type of table to create.
      using Table = T;

      //! Constructs a create table statement.
      /*!
        \param t The table to create.
        \param name The name of the table.
        \param exists_flag Only create the table if it doesn't exist.
      */
      CreateTableStatement(Table t, std::string name, bool exists_flag);

      //! Returns the table to create.
      const Table& get_table() const;

      //! Returns the name of the table.
      const std::string& get_name() const;

      //! Whether the table should be created only if it doesn't exist yet.
      bool get_exists_flag() const;

    private:
      Table m_table;
      std::string m_name;
      bool m_exists_flag;
  };

  //! Builds a create table statement.
  /*!
    \param t The table to create.
    \param name The name of the table.
  */
  template<typename T>
  auto create(Table<T> t, std::string name) {
    return CreateTableStatement(std::move(t), std::move(name), false);
  }

  //! Builds a create table statement if it doesn't already exist.
  /*!
    \param t The table to create.
    \param name The name of the table.
  */
  template<typename T>
  auto create_if_not_exists(Table<T> t, std::string name) {
    return CreateTableStatement(std::move(t), std::move(name), true);
  }

  template<typename T>
  CreateTableStatement<T>::CreateTableStatement(Table t, std::string name,
      bool exists_flag)
      : m_table(std::move(t)),
        m_name(std::move(name)),
        m_exists_flag(exists_flag) {}

  template<typename T>
  const typename CreateTableStatement<T>::Table&
      CreateTableStatement<T>::get_table() const {
    return m_table;
  }

  template<typename T>
  const std::string& CreateTableStatement<T>::get_name() const {
    return m_name;
  }

  template<typename T>
  bool CreateTableStatement<T>::get_exists_flag() const {
    return m_exists_flag;
  }
}

#endif
