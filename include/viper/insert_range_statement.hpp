#ifndef VIPER_INSERT_RANGE_STATEMENT_HPP
#define VIPER_INSERT_RANGE_STATEMENT_HPP
#include <string>
#include "viper/table.hpp"

namespace Viper {

  /*! \brief Represents an SQL INSERT statement for a range of values.
      \tparam T The type table to insert into.
      \tparam B The type of input iterator to the beginning of the range to
                insert.
      \tparam E The type of input iterator to the end of the range to insert.
   */
  template<typename T, typename B, typename E>
  class InsertRangeStatement {
    public:

      //! The type of table to insert into.
      using Table = T;

      //! The type of input iterator to the beginning of the range to insert.
      using Begin = B;

      //! The type of input iterator to the end of the range to insert.
      using End = E;

      //! Constructs an insert range statement.
      /*!
        \param t The table to insert into.
        \param into The table to insert into.
        \param begin An input iterator to the beginning of the range to insert.
        \param end An input iterator to the end of the range to insert.
      */
      InsertRangeStatement(Table t, std::string into, Begin begin, End end);

      //! Returns the table to insert into.
      const Table& get_from_table() const;

      //! Returns the table to insert into.
      const std::string& get_into_table() const;

      //! Returns the begin iterator.
      Begin get_begin() const;

      //! Returns the end iterator.
      End get_end() const;

    private:
      Table m_table;
      std::string m_into_table;
      Begin m_begin;
      End m_end;
  };

  //! Builds an insert range statement.
  /*!
    \param t The table to insert into.
    \param into The table to insert into.
    \param begin An input iterator to the beginning of the range to insert.
    \param end An input iterator to the end of the range to insert.
  */
  template<typename T, typename B, typename E>
  auto insert(T t, std::string into, B begin, E end) {
    return InsertRangeStatement(std::move(t), std::move(into),
      std::move(begin), std::move(end));
  }

  template<typename T, typename B, typename E>
  InsertRangeStatement<T, B, E>::InsertRangeStatement(Table t, std::string into,
      Begin begin, End end)
      : m_table(std::move(t)),
        m_into_table(std::move(into)),
        m_begin(std::move(begin)),
        m_end(std::move(end)) {}

  template<typename T, typename B, typename E>
  const typename InsertRangeStatement<T, B, E>::Table&
      InsertRangeStatement<T, B, E>::get_from_table() const {
    return m_table;
  }

  template<typename T, typename B, typename E>
  const std::string& InsertRangeStatement<T, B, E>::get_into_table() const {
    return m_into_table;
  }

  template<typename T, typename B, typename E>
  typename InsertRangeStatement<T, B, E>::Begin
      InsertRangeStatement<T, B, E>::get_begin() const {
    return m_begin;
  }

  template<typename T, typename B, typename E>
  typename InsertRangeStatement<T, B, E>::End
      InsertRangeStatement<T, B, E>::get_end() const {
    return m_end;
  }
}

#endif
