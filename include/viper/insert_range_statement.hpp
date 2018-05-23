#ifndef VIPER_INSERT_RANGE_STATEMENT_HPP
#define VIPER_INSERT_RANGE_STATEMENT_HPP
#include <string>
#include "viper/table.hpp"

namespace viper {

  /*! \brief Represents an SQL INSERT statement for a range of values.
      \tparam T The type table to insert into.
      \tparam B The type of input iterator to the beginning of the range to
                insert.
      \tparam E The type of input iterator to the end of the range to insert.
   */
  template<typename T, typename B, typename E>
  class insert_range_statement {
    public:

      //! The type of table to insert into.
      using table = T;

      //! The type of input iterator to the beginning of the range to insert.
      using begin_t = B;

      //! The type of input iterator to the end of the range to insert.
      using end_t = E;

      //! Constructs an insert range statement.
      /*!
        \param t The table to insert into.
        \param into The table to insert into.
        \param begin An input iterator to the beginning of the range to insert.
        \param end An input iterator to the end of the range to insert.
      */
      insert_range_statement(table t, std::string into, begin_t begin,
        end_t end);

      //! Returns the table to insert into.
      const table& get_table() const;

      //! Returns the table to insert into.
      const std::string& get_into_table() const;

      //! Returns the begin iterator.
      begin_t get_begin() const;

      //! Returns the end iterator.
      end_t get_end() const;

    private:
      table m_table;
      std::string m_into_table;
      begin_t m_begin;
      end_t m_end;
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
    return insert_range_statement(std::move(t), std::move(into),
      std::move(begin), std::move(end));
  }

  template<typename T, typename B, typename E>
  insert_range_statement<T, B, E>::insert_range_statement(table t,
      std::string into, begin_t begin, end_t end)
      : m_table(std::move(t)),
        m_into_table(std::move(into)),
        m_begin(std::move(begin)),
        m_end(std::move(end)) {}

  template<typename T, typename B, typename E>
  const typename insert_range_statement<T, B, E>::table&
      insert_range_statement<T, B, E>::get_table() const {
    return m_table;
  }

  template<typename T, typename B, typename E>
  const std::string& insert_range_statement<T, B, E>::get_into_table() const {
    return m_into_table;
  }

  template<typename T, typename B, typename E>
  typename insert_range_statement<T, B, E>::begin_t
      insert_range_statement<T, B, E>::get_begin() const {
    return m_begin;
  }

  template<typename T, typename B, typename E>
  typename insert_range_statement<T, B, E>::end_t
      insert_range_statement<T, B, E>::get_end() const {
    return m_end;
  }
}

#endif
