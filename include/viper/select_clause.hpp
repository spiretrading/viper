#ifndef VIPER_SELECT_CLAUSE_HPP
#define VIPER_SELECT_CLAUSE_HPP
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>
#include "viper/expressions/expression.hpp"
#include "viper/utilities.hpp"

namespace Viper {
  class select_clause;

  //! Represents a limit on the number of rows produced by a query.
  struct limit {

    //! The maximum number of rows to return.
    int m_value;

    limit(int value);
  };

  //! Represents the order in which results are returned.
  struct order {

    //! Rows are in ascending (increasing) order.
    static constexpr auto ASC = 0;

    //! Rows are in descending (decreasing) order.
    static constexpr auto DESC = 1;

    //! The columns to order by.
    std::vector<std::string> m_columns;

    //! The order of the rows.
    int m_order;

    order(std::vector<std::string> columns, int o);
  };

  //! Represents a SELECT query's from clause.
  struct from_clause :
      std::variant<std::string, std::shared_ptr<select_clause>> {
    using variant = std::variant<std::string, std::shared_ptr<select_clause>>;

    using variant::variant;

    //! Constructs a recursive select clause.
    from_clause(select_clause s);
  };

  //! Represents an SQL SELECT sub-query/clause.
  class select_clause {
    public:

      //! Constructs a select clause.
      /*!
        \param columns The list of columns to select.
        \param from The table to select from.
      */
      select_clause(std::vector<std::string> columns, from_clause from);

      //! Constructs a select statement with a single optional clause.
      /*!
        \param columns The list of columns to select.
        \param from The table to select from.
        \param c1 The first clause to include.
      */
      template<typename C1>
      select_clause(std::vector<std::string> columns, from_clause from,
        C1&& c1);

      //! Constructs a select statement with a two optional clauses.
      /*!
        \param columns The list of columns to select.
        \param from The table to select from.
        \param c1 The first clause to include.
        \param c2 The second clause to include.
      */
      template<typename C1, typename C2>
      select_clause(std::vector<std::string> columns, from_clause from, C1&& c1,
        C2&& c2);

      //! Constructs a select statement with a three optional clauses.
      /*!
        \param columns The list of columns to select.
        \param from The table to select from.
        \param c1 The first clause to include.
        \param c2 The second clause to include.
        \param c3 The third clause to include.
      */
      template<typename C1, typename C2, typename C3>
      select_clause(std::vector<std::string> columns, from_clause from, C1&& c1,
        C2&& c2, C3&& c3);

      //! Returns the list columns being selected.
      const std::vector<std::string>& get_columns() const;

      //! Returns the table to select from.
      const from_clause& get_from() const;

      //! Returns the where clause.
      const std::optional<Expression>& get_where() const;

      //! Returns the order by clause.
      const std::optional<order>& get_order() const;

      //! Returns the limit.
      const std::optional<limit>& get_limit() const;

    private:
      std::vector<std::string> m_columns;
      from_clause m_from;
      std::optional<Expression> m_where;
      std::optional<order> m_order;
      std::optional<limit> m_limit;
  };

  //! Builds an ORDER BY clause.
  inline order order_by(std::vector<std::string> columns, int o) {
    return order(std::move(columns), o);
  }

  //! Builds an ORDER BY clause.
  inline order order_by(std::string_view column, int o) {
    std::vector<std::string> c;
    c.emplace_back(column);
    return order_by(std::move(c), o);
  }

  //! Builds a select clause.
  /*!
    \param columns The list of columns to select.
    \param from The table to select from.
  */
  inline select_clause select(std::vector<std::string> columns,
      from_clause from) {
    return select_clause(std::move(columns), std::move(from));
  }

  //! Builds a select clause with a single optional clause.
  /*!
    \param columns The list of columns to select.
    \param from The table to select from.
    \param c1 The first clause.
  */
  template<typename C1>
  select_clause select(std::vector<std::string> columns, from_clause from,
      C1&& c1) {
    return select_clause(std::move(columns), std::move(from), std::move(c1));
  }

  //! Builds a select statement with two optional clauses.
  /*!
    \param columns The list of columns to select.
    \param from The table to select from.
    \param c1 The first clause.
    \param c2 The second clause.
  */
  template<typename C1, typename C2>
  select_clause select(std::vector<std::string> columns, from_clause from,
      C1&& c1, C2&& c2) {
    return select_clause(std::move(columns), std::move(from), std::move(c1),
      std::move(c2));
  }

  //! Builds a select statement with three optional clauses.
  /*!
    \param columns The list of columns to select.
    \param from The table to select from.
    \param c1 The first clause.
    \param c2 The second clause.
    \param c3 The third clause.
  */
  template<typename C1, typename C2, typename C3>
  select_clause select(std::vector<std::string> columns, from_clause from,
      C1&& c1, C2&& c2, C3&& c3) {
    return select_clause(std::move(columns), std::move(from), std::move(c1),
      std::move(c2), std::move(c3));
  }

  inline limit::limit(int value)
      : m_value(value) {}

  inline order::order(std::vector<std::string> columns, int o)
      : m_columns(std::move(columns)),
        m_order(o) {}

  inline from_clause::from_clause(select_clause s)
      : variant(std::make_shared<select_clause>(s)) {}

  inline select_clause::select_clause(std::vector<std::string> columns,
      from_clause from)
      : m_columns(std::move(columns)),
        m_from(std::move(from)) {}

  template<typename C1>
  select_clause::select_clause(std::vector<std::string> columns,
      from_clause from, C1&& c1)
      : m_columns(std::move(columns)),
        m_from(std::move(from)),
        m_where(move_if<std::is_constructible_v<Expression, C1&&>>(
          std::move(c1), std::nullopt)),
        m_order(move_if<std::is_constructible_v<order, C1&&>>(
          std::move(c1), std::nullopt)),
        m_limit(move_if<std::is_constructible_v<limit, C1&&>>(
          std::move(c1), std::nullopt)) {}

  template<typename C1, typename C2>
  select_clause::select_clause(std::vector<std::string> columns,
      from_clause from, C1&& c1, C2&& c2)
      : m_columns(std::move(columns)),
        m_from(std::move(from)),
        m_where(move_if<std::is_constructible_v<Expression, C1&&>,
          std::is_constructible_v<Expression, C2&&>>(std::move(c1),
          std::move(c2), std::nullopt)),
        m_order(move_if<std::is_constructible_v<order, C1&&>,
          std::is_constructible_v<order, C2&&>>(std::move(c1), std::move(c2),
          std::nullopt)),
        m_limit(move_if<std::is_constructible_v<limit, C1&&>,
          std::is_constructible_v<limit, C2&&>>(std::move(c1), std::move(c2),
          std::nullopt)) {}

  template<typename C1, typename C2, typename C3>
  select_clause::select_clause(std::vector<std::string> columns,
      from_clause from, C1&& c1, C2&& c2, C3&& c3)
      : m_columns(std::move(columns)),
        m_from(std::move(from)),
        m_where(move_if<std::is_constructible_v<Expression, C1&&>,
          std::is_constructible_v<Expression, C2&&>,
          std::is_constructible_v<Expression, C3&&>>(std::move(c1),
          std::move(c2), std::move(c3), std::nullopt)),
        m_order(move_if<std::is_constructible_v<order, C1&&>,
          std::is_constructible_v<order, C2&&>,
          std::is_constructible_v<order, C3&&>>(std::move(c1), std::move(c2),
          std::move(c3), std::nullopt)),
        m_limit(move_if<std::is_constructible_v<limit, C1&&>,
          std::is_constructible_v<limit, C2&&>,
          std::is_constructible_v<limit, C3&&>>(std::move(c1), std::move(c2),
          std::move(c3), std::nullopt)) {}

  inline const std::vector<std::string>& select_clause::get_columns() const {
    return m_columns;
  }

  inline const from_clause& select_clause::get_from() const {
    return m_from;
  }

  inline const std::optional<Expression>& select_clause::get_where() const {
    return m_where;
  }

  inline const std::optional<order>& select_clause::get_order() const {
    return m_order;
  }

  inline const std::optional<limit>& select_clause::get_limit() const {
    return m_limit;
  }
}

#endif
