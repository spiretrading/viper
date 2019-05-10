#ifndef VIPER_INSERT_RANGE_STATEMENT_HPP
#define VIPER_INSERT_RANGE_STATEMENT_HPP
#include <string>
#include "Viper/Row.hpp"

namespace Viper {

  /*! \brief Represents an SQL INSERT statement for a range of values.
      \tparam R The type row to insert.
      \tparam B The type of input iterator to the beginning of the range to
                insert.
      \tparam E The type of input iterator to the end of the range to insert.
   */
  template<typename R, typename B, typename E>
  class InsertRangeStatement {
    public:

      //! The type of row to insert.
      using Row = R;

      //! The type of input iterator to the beginning of the range to insert.
      using Begin = B;

      //! The type of input iterator to the end of the range to insert.
      using End = E;

      //! Constructs an insert range statement.
      /*!
        \param row The type of row to insert.
        \param table The name of the table to insert into.
        \param begin An input iterator to the beginning of the range to insert.
        \param end An input iterator to the end of the range to insert.
      */
      InsertRangeStatement(Row row, std::string table, Begin begin, End end);

      //! Returns the type of row to insert.
      const Row& get_row() const;

      //! Returns the name of the table to insert into.
      const std::string& get_table() const;

      //! Returns the begin iterator.
      Begin get_begin() const;

      //! Returns the end iterator.
      End get_end() const;

    private:
      Row m_row;
      std::string m_table;
      Begin m_begin;
      End m_end;
  };

  //! Builds an insert range statement.
  /*!
    \param row The type of row to insert.
    \param table The name of the table to insert into.
    \param begin An input iterator to the beginning of the range to insert.
    \param end An input iterator to the end of the range to insert.
  */
  template<typename R, typename B, typename E>
  auto insert(R row, std::string table, B begin, E end) {
    return InsertRangeStatement(std::move(row), std::move(table),
      std::move(begin), std::move(end));
  }

  //! Builds an insert statement for a single value.
  /*!
    \param row The type of row to insert.
    \param table The name of the table to insert into.
    \param value A pointer to the value to insert.
  */
  template<typename R, typename T>
  auto insert(R row, std::string table, const T* value) {
    return InsertRangeStatement(std::move(row), std::move(table), value,
      value + 1);
  }

  template<typename R, typename B, typename E>
  InsertRangeStatement<R, B, E>::InsertRangeStatement(Row row,
      std::string table, Begin begin, End end)
      : m_row(std::move(row)),
        m_table(std::move(table)),
        m_begin(std::move(begin)),
        m_end(std::move(end)) {}

  template<typename R, typename B, typename E>
  const typename InsertRangeStatement<R, B, E>::Row&
      InsertRangeStatement<R, B, E>::get_row() const {
    return m_row;
  }

  template<typename R, typename B, typename E>
  const std::string& InsertRangeStatement<R, B, E>::get_table() const {
    return m_table;
  }

  template<typename R, typename B, typename E>
  typename InsertRangeStatement<R, B, E>::Begin
      InsertRangeStatement<R, B, E>::get_begin() const {
    return m_begin;
  }

  template<typename R, typename B, typename E>
  typename InsertRangeStatement<R, B, E>::End
      InsertRangeStatement<R, B, E>::get_end() const {
    return m_end;
  }
}

#endif
