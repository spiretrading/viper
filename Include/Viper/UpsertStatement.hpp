#ifndef VIPER_UPSERT_STATEMENT_HPP
#define VIPER_UPSERT_STATEMENT_HPP
#include <string>
#include "Viper/Row.hpp"

namespace Viper {

  /*! \brief Inserts a row or updates the existing row.
      \tparam R The type row to upsert.
      \tparam B The type of input iterator to the beginning of the range to
                upsert.
      \tparam E The type of input iterator to the end of the range to upsert.
   */
  template<typename R, typename B, typename E>
  class UpsertStatement {
    public:

      //! The type of row to upsert.
      using Row = R;

      //! The type of input iterator to the beginning of the range to upsert.
      using Begin = B;

      //! The type of input iterator to the end of the range to upsert.
      using End = E;

      //! Constructs an upsert range statement.
      /*!
        \param row The type of row to upsert.
        \param table The name of the table to upsert into.
        \param begin An input iterator to the beginning of the range to upsert.
        \param end An input iterator to the end of the range to upsert.
      */
      UpsertStatement(Row row, std::string table, Begin begin, End end);

      //! Returns the type of row to upsert.
      const Row& get_row() const;

      //! Returns the name of the table to upsert into.
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

  //! Builds an upsert range statement.
  /*!
    \param row The type of row to upsert.
    \param table The name of the table to upsert into.
    \param begin An input iterator to the beginning of the range to upsert.
    \param end An input iterator to the end of the range to upsert.
  */
  template<typename R, typename B, typename E>
  auto upsert(R row, std::string table, B begin, E end) {
    return UpsertStatement(std::move(row), std::move(table), std::move(begin),
      std::move(end));
  }

  //! Builds an upsert statement for a single value.
  /*!
    \param row The type of row to upsert.
    \param table The name of the table to upsert into.
    \param value A pointer to the value to upsert.
  */
  template<typename R, typename T>
  auto upsert(R row, std::string table, const T* value) {
    return UpsertStatement(std::move(row), std::move(table), value, value + 1);
  }

  template<typename R, typename B, typename E>
  UpsertStatement<R, B, E>::UpsertStatement(Row row, std::string table,
      Begin begin, End end)
      : m_row(std::move(row)),
        m_table(std::move(table)),
        m_begin(std::move(begin)),
        m_end(std::move(end)) {}

  template<typename R, typename B, typename E>
  const typename UpsertStatement<R, B, E>::Row&
      UpsertStatement<R, B, E>::get_row() const {
    return m_row;
  }

  template<typename R, typename B, typename E>
  const std::string& UpsertStatement<R, B, E>::get_table() const {
    return m_table;
  }

  template<typename R, typename B, typename E>
  typename UpsertStatement<R, B, E>::Begin
      UpsertStatement<R, B, E>::get_begin() const {
    return m_begin;
  }

  template<typename R, typename B, typename E>
  typename UpsertStatement<R, B, E>::End
      UpsertStatement<R, B, E>::get_end() const {
    return m_end;
  }
}

#endif
