#ifndef VIPER_SELECT_STATEMENT_HPP
#define VIPER_SELECT_STATEMENT_HPP
#include "Viper/Row.hpp"
#include "Viper/SelectClause.hpp"

namespace Viper {

  /*! \brief Represents an SQL SELECT statement.
      \tparam R The type of row to select.
      \tparam D The output iterator to store the rows in.
   */
  template<typename R, typename D>
  class SelectStatement {
    public:

      //! The type of row to select.
      using Row = R;

      //! The output iterator to store the rows in.
      using Destination = D;

      //! Constructs a select statement.
      /*!
        \param row The type of row to select.
        \param clause The select clause to execute.
        \param first An output iterator used to store the rows.
      */
      SelectStatement(Row row, SelectClause clause, Destination first);

      //! Returns the type of row to select.
      const Row& get_row() const;

      //! Returns the select clause.
      const SelectClause& get_clause() const;

      //! Returns the destination iterator.
      Destination get_first() const;

    private:
      Row m_row;
      SelectClause m_clause;
      Destination m_first;
  };

  //! Builds a select statement.
  /*!
    \param row The type of row to select.
    \param from The table to select from.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D>
  auto select(Row<T> row, FromClause from, D first) {
    std::vector<std::string> columns;
    for(auto& c : row.get_columns()) {
      columns.push_back(c.m_name);
    }
    return SelectStatement(std::move(row), select(std::move(columns),
      std::move(from)), std::move(first));
  }

  //! Builds a select statement with a single clause.
  /*!
    \param row The type of row to select.
    \param from The table to select from.
    \param c1 The first clause.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D, typename C1>
  auto select(Row<T> row, FromClause from, C1&& c1, D first) {
    std::vector<std::string> columns;
    for(auto& c : row.get_columns()) {
      columns.push_back(c.m_name);
    }
    return SelectStatement(std::move(row), select(std::move(columns),
      std::move(from), std::move(c1)), std::move(first));
  }

  //! Builds a select statement with two clauses.
  /*!
    \param row The type of row to select.
    \param from The table to select from.
    \param c1 The first clause.
    \param c2 The second clause.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D, typename C1, typename C2>
  auto select(Row<T> row, FromClause from, C1&& c1, C2&& c2, D first) {
    std::vector<std::string> columns;
    for(auto& c : row.get_columns()) {
      columns.push_back(c.m_name);
    }
    return SelectStatement(std::move(row), select(std::move(columns),
      std::move(from), std::move(c1), std::move(c2)), std::move(first));
  }

  //! Builds a select statement with three clauses.
  /*!
    \param row The type of row to select.
    \param from The table to select from.
    \param c1 The first clause.
    \param c2 The second clause.
    \param c3 The third clause.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D, typename C1, typename C2, typename C3>
  auto select(Row<T> row, FromClause from, C1&& c1, C2&& c2, C3&& c3, D first) {
    std::vector<std::string> columns;
    for(auto& c : row.get_columns()) {
      columns.push_back(c.m_name);
    }
    return SelectStatement(std::move(row), select(std::move(columns),
      std::move(from), std::move(c1), std::move(c2), std::move(c3)),
      std::move(first));
  }

  template<typename R, typename D>
  SelectStatement<R, D>::SelectStatement(Row row, SelectClause clause,
      Destination first)
      : m_row(std::move(row)),
        m_clause(std::move(clause)),
        m_first(std::move(first)) {}

  template<typename R, typename D>
  const typename SelectStatement<R, D>::Row&
      SelectStatement<R, D>::get_row() const {
    return m_row;
  }

  template<typename R, typename D>
  const SelectClause& SelectStatement<R, D>::get_clause() const {
    return m_clause;
  }

  template<typename R, typename D>
  typename SelectStatement<R, D>::Destination
      SelectStatement<R, D>::get_first() const {
    return m_first;
  }
}

#endif
