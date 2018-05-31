#ifndef VIPER_SELECT_STATEMENT_HPP
#define VIPER_SELECT_STATEMENT_HPP
#include <optional>
#include <string>
#include "viper/expressions/expression.hpp"
#include "viper/table.hpp"

namespace viper {

  /*! \brief Represents an SQL SELECT statement.
      \tparam T The type of the result table.
      \tparam D The output iterator to store the rows in.
   */
  template<typename T, typename D>
  class select_statement {
    public:

      //! The type of the result table.
      using result_table = T;

      //! The output iterator to store the rows in.
      using destination = D;

      //! Constructs a select statement.
      /*!
        \param r The result table.
        \param from The table to select from.
        \param first An output iterator used to store the rows.
      */
      select_statement(result_table r, std::string from, destination first);

      //! Constructs a select statement.
      /*!
        \param r The result table.
        \param from The table to select from.
        \param where The where clause.
        \param first An output iterator used to store the rows.
      */
      select_statement(result_table r, std::string from, expression where,
        destination first);

      //! Returns the result table.
      const result_table& get_result_table() const;

      //! Returns the table to select from.
      const std::string& get_from_table() const;

      //! Returns the where clause.
      const std::optional<expression>& get_where() const;

      //! Returns the destination iterator.
      destination get_first() const;

    private:
      result_table m_result_table;
      std::string m_from_table;
      std::optional<expression> m_where;
      destination m_first;
  };

  //! Builds a select statement.
  /*!
    \param r The result table.
    \param from The table to select from.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D>
  auto select(T r, std::string from, D first) {
    return select_statement(std::move(r), std::move(from), std::move(first));
  }

  //! Builds a select statement.
  /*!
    \param r The result table.
    \param from The table to select from.
    \param where The where clause.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D>
  auto select(T r, std::string from, expression where, D first) {
    return select_statement(std::move(r), std::move(from), std::move(where),
      std::move(first));
  }

  template<typename T, typename D>
  select_statement<T, D>::select_statement(result_table r, std::string from,
      destination first)
      : m_result_table(std::move(r)),
        m_from_table(std::move(from)),
        m_first(std::move(first)) {}

  template<typename T, typename D>
  select_statement<T, D>::select_statement(result_table r, std::string from,
      expression where, destination first)
      : m_result_table(std::move(r)),
        m_from_table(std::move(from)),
        m_where(std::move(where)),
        m_first(std::move(first)) {}

  template<typename T, typename D>
  const typename select_statement<T, D>::result_table&
      select_statement<T, D>::get_result_table() const {
    return m_result_table;
  }

  template<typename T, typename D>
  const std::string& select_statement<T, D>::get_from_table() const {
    return m_from_table;
  }

  template<typename T, typename D>
  const std::optional<expression>& select_statement<T, D>::get_where() const {
    return m_where;
  }

  template<typename T, typename D>
  typename select_statement<T, D>::destination
      select_statement<T, D>::get_first() const {
    return m_first;
  }
}

#endif
