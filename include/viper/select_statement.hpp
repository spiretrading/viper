#ifndef VIPER_SELECT_STATEMENT_HPP
#define VIPER_SELECT_STATEMENT_HPP
#include <string_view>
#include "viper/row.hpp"

namespace viper {

  /*! \brief Represents an SQL SELECT statement.
      \tparam T The type of row to select.
      \tparam D The output iterator to store the rows in.
   */
  template<typename T, typename D>
  class select_statement {
    public:

      //! The type of row to select.
      using type = T;

      //! The output iterator to store the rows in.
      using destination = D;

      //! Constructs a select statement.
      /*!
        \param r The row to select.
        \param table The table to select from.
        \param first An output iterator used to store the rows.
      */
      select_statement(row<type> r, std::string_view table, destination first);

      //! Returns the row to select.
      const row<type>& get_row() const;

      //! Returns the table to select from.
      const std::string& get_table() const;

      //! Returns the destination iterator.
      destination get_first() const;

    private:
      row<type> m_row;
      std::string m_table;
      destination m_first;
  };

  //! Builds a select statement.
  /*!
    \param r The row to select.
    \param table The table to select from.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D>
  auto select(row<T> r, std::string_view table, D first) {
    return select_statement(std::move(r), table, std::move(first));
  }

  template<typename T, typename D>
  select_statement<T, D>::select_statement(row<type> r, std::string_view table,
      destination first)
      : m_row(std::move(r)),
        m_table(table),
        m_first(std::move(first)) {}

  template<typename T, typename D>
  const row<typename select_statement<T, D>::type>&
      select_statement<T, D>::get_row() const {
    return m_row;
  }

  template<typename T, typename D>
  const std::string& select_statement<T, D>::get_table() const {
    return m_table;
  }

  template<typename T, typename D>
  typename select_statement<T, D>::destination
      select_statement<T, D>::get_first() const {
    return m_first;
  }
}

#endif
