#ifndef VIPER_SELECT_STATEMENT_HPP
#define VIPER_SELECT_STATEMENT_HPP
#include "viper/select_clause.hpp"
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
        \param clause The select clause to execute.
        \param first An output iterator used to store the rows.
      */
      select_statement(result_table r, select_clause clause, destination first);

      //! Returns the result table.
      const result_table& get_result_table() const;

      //! Returns the select clause.
      const select_clause& get_clause() const;

      //! Returns the destination iterator.
      destination get_first() const;

    private:
      result_table m_result_table;
      select_clause m_clause;
      destination m_first;
  };

  //! Builds a select statement.
  /*!
    \param r The result table.
    \param from The table to select from.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D>
  auto select(table<T> r, from_clause from, D first) {
    std::vector<std::string> columns;
    for(auto& c : r.get_columns()) {
      columns.push_back(c.m_name);
    }
    return select_statement(std::move(r), select(std::move(columns),
      std::move(from)), std::move(first));
  }

  //! Builds a select statement with a single clause.
  /*!
    \param r The result table.
    \param from The table to select from.
    \param c1 The first clause.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D, typename C1>
  auto select(table<T> r, from_clause from, C1&& c1, D first) {
    std::vector<std::string> columns;
    for(auto& c : r.get_columns()) {
      columns.push_back(c.m_name);
    }
    return select_statement(std::move(r), select(std::move(columns),
      std::move(from), std::move(c1)), std::move(first));
  }

  //! Builds a select statement with two clauses.
  /*!
    \param r The result table.
    \param from The table to select from.
    \param c1 The first clause.
    \param c2 The second clause.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D, typename C1, typename C2>
  auto select(table<T> r, from_clause from, C1&& c1, C2&& c2, D first) {
    std::vector<std::string> columns;
    for(auto& c : r.get_columns()) {
      columns.push_back(c.m_name);
    }
    return select_statement(std::move(r), select(std::move(columns),
      std::move(from), std::move(c1), std::move(c2)), std::move(first));
  }

  //! Builds a select statement with three clauses.
  /*!
    \param r The result table.
    \param from The table to select from.
    \param c1 The first clause.
    \param c2 The second clause.
    \param c3 The third clause.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D, typename C1, typename C2, typename C3>
  auto select(table<T> r, from_clause from, C1&& c1, C2&& c2, C3&& c3,
      D first) {
    std::vector<std::string> columns;
    for(auto& c : r.get_columns()) {
      columns.push_back(c.m_name);
    }
    return select_statement(std::move(r), select(std::move(columns),
      std::move(from), std::move(c1), std::move(c2), std::move(c3)),
      std::move(first));
  }

  template<typename T, typename D>
  select_statement<T, D>::select_statement(result_table r, select_clause clause,
      destination first)
      : m_result_table(std::move(r)),
        m_clause(std::move(clause)),
        m_first(std::move(first)) {}

  template<typename T, typename D>
  const typename select_statement<T, D>::result_table&
      select_statement<T, D>::get_result_table() const {
    return m_result_table;
  }

  template<typename T, typename D>
  const select_clause& select_statement<T, D>::get_clause() const {
    return m_clause;
  }

  template<typename T, typename D>
  typename select_statement<T, D>::destination
      select_statement<T, D>::get_first() const {
    return m_first;
  }
}

#endif
