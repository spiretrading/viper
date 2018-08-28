#ifndef VIPER_CREATE_TABLE_STATEMENT_HPP
#define VIPER_CREATE_TABLE_STATEMENT_HPP
#include <string>
#include "Viper/Row.hpp"

namespace Viper {

  /*! \brief Represents an SQL CREATE TABLE statement.
      \tparam R The type of the table's row.
   */
  template<typename R>
  class CreateTableStatement {
    public:

      //! The type of the table's row.
      using Row = R;

      //! Constructs a create table statement.
      /*!
        \param row The table's row to create.
        \param name The name of the table.
        \param exists_flag Only create the table if it doesn't exist.
      */
      CreateTableStatement(Row row, std::string name, bool exists_flag);

      //! Returns the table row to create.
      const Row& get_row() const;

      //! Returns the name of the table.
      const std::string& get_name() const;

      //! Whether the table should be created only if it doesn't exist yet.
      bool get_exists_flag() const;

    private:
      Row m_row;
      std::string m_name;
      bool m_exists_flag;
  };

  //! Builds a create table statement.
  /*!
    \param row The table's row to create.
    \param name The name of the table.
  */
  template<typename T>
  auto create(Row<T> row, std::string name) {
    return CreateTableStatement(std::move(row), std::move(name), false);
  }

  //! Builds a create table statement if it doesn't already exist.
  /*!
    \param row The table's row to create.
    \param name The name of the table.
  */
  template<typename T>
  auto create_if_not_exists(Row<T> row, std::string name) {
    return CreateTableStatement(std::move(row), std::move(name), true);
  }

  template<typename R>
  CreateTableStatement<R>::CreateTableStatement(Row row, std::string name,
      bool exists_flag)
      : m_row(std::move(row)),
        m_name(std::move(name)),
        m_exists_flag(exists_flag) {}

  template<typename R>
  const typename CreateTableStatement<R>::Row&
      CreateTableStatement<R>::get_row() const {
    return m_row;
  }

  template<typename R>
  const std::string& CreateTableStatement<R>::get_name() const {
    return m_name;
  }

  template<typename R>
  bool CreateTableStatement<R>::get_exists_flag() const {
    return m_exists_flag;
  }
}

#endif
