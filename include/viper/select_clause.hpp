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
  class SelectClause;

  //! Represents a limit on the number of rows produced by a query.
  struct Limit {

    //! The maximum number of rows to return.
    int m_value;

    Limit(int value);
  };

  //! Returns a limit on the number of rows produced by a query.
  inline auto limit(int value) {
    return Limit(value);
  }

  //! Represents the order in which results are returned.
  struct Order {

    //! Rows are in ascending (increasing) order.
    static constexpr auto ASC = 0;

    //! Rows are in descending (decreasing) order.
    static constexpr auto DESC = 1;

    //! The columns to order by.
    std::vector<std::string> m_columns;

    //! The order of the rows.
    int m_order;

    Order(std::vector<std::string> columns, int order);
  };

  //! Represents a SELECT query's from clause.
  struct FromClause : std::variant<std::string, std::shared_ptr<SelectClause>> {
    using variant = std::variant<std::string, std::shared_ptr<SelectClause>>;

    using variant::variant;

    //! Constructs a recursive select clause.
    FromClause(SelectClause s);
  };

  //! Represents an SQL SELECT sub-query/clause.
  class SelectClause {
    public:

      //! Constructs a select clause.
      /*!
        \param columns The list of columns to select.
        \param from The table to select from.
      */
      SelectClause(std::vector<std::string> columns, FromClause from);

      //! Constructs a select statement with a single optional clause.
      /*!
        \param columns The list of columns to select.
        \param from The table to select from.
        \param c1 The first clause to include.
      */
      template<typename C1>
      SelectClause(std::vector<std::string> columns, FromClause from, C1&& c1);

      //! Constructs a select statement with a two optional clauses.
      /*!
        \param columns The list of columns to select.
        \param from The table to select from.
        \param c1 The first clause to include.
        \param c2 The second clause to include.
      */
      template<typename C1, typename C2>
      SelectClause(std::vector<std::string> columns, FromClause from, C1&& c1,
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
      SelectClause(std::vector<std::string> columns, FromClause from, C1&& c1,
        C2&& c2, C3&& c3);

      //! Returns the list columns being selected.
      const std::vector<std::string>& get_columns() const;

      //! Returns the table to select from.
      const FromClause& get_from() const;

      //! Returns the where clause.
      const std::optional<Expression>& get_where() const;

      //! Returns the order by clause.
      const std::optional<Order>& get_order() const;

      //! Returns the limit.
      const std::optional<Limit>& get_limit() const;

    private:
      std::vector<std::string> m_columns;
      FromClause m_from;
      std::optional<Expression> m_where;
      std::optional<Order> m_order;
      std::optional<Limit> m_limit;
  };

  //! Builds an ORDER BY clause.
  inline Order order_by(std::vector<std::string> columns, int o) {
    return Order(std::move(columns), o);
  }

  //! Builds an ORDER BY clause.
  inline Order order_by(std::string_view column, int o) {
    std::vector<std::string> c;
    c.emplace_back(column);
    return order_by(std::move(c), o);
  }

  //! Builds a select clause.
  /*!
    \param columns The list of columns to select.
    \param from The table to select from.
  */
  inline SelectClause select(std::vector<std::string> columns,
      FromClause from) {
    return SelectClause(std::move(columns), std::move(from));
  }

  //! Builds a select clause with a single optional clause.
  /*!
    \param columns The list of columns to select.
    \param from The table to select from.
    \param c1 The first clause.
  */
  template<typename C1>
  SelectClause select(std::vector<std::string> columns, FromClause from,
      C1&& c1) {
    return SelectClause(std::move(columns), std::move(from), std::move(c1));
  }

  //! Builds a select statement with two optional clauses.
  /*!
    \param columns The list of columns to select.
    \param from The table to select from.
    \param c1 The first clause.
    \param c2 The second clause.
  */
  template<typename C1, typename C2>
  SelectClause select(std::vector<std::string> columns, FromClause from,
      C1&& c1, C2&& c2) {
    return SelectClause(std::move(columns), std::move(from), std::move(c1),
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
  SelectClause select(std::vector<std::string> columns, FromClause from,
      C1&& c1, C2&& c2, C3&& c3) {
    return SelectClause(std::move(columns), std::move(from), std::move(c1),
      std::move(c2), std::move(c3));
  }

  inline Limit::Limit(int value)
      : m_value(value) {}

  inline Order::Order(std::vector<std::string> columns, int o)
      : m_columns(std::move(columns)),
        m_order(o) {}

  inline FromClause::FromClause(SelectClause s)
      : variant(std::make_shared<SelectClause>(s)) {}

  inline SelectClause::SelectClause(std::vector<std::string> columns,
      FromClause from)
      : m_columns(std::move(columns)),
        m_from(std::move(from)) {}

  template<typename C1>
  SelectClause::SelectClause(std::vector<std::string> columns,
      FromClause from, C1&& c1)
      : m_columns(std::move(columns)),
        m_from(std::move(from)),
        m_where(move_if<std::is_constructible_v<Expression, C1&&>>(
          std::move(c1), std::nullopt)),
        m_order(move_if<std::is_constructible_v<Order, C1&&>>(
          std::move(c1), std::nullopt)),
        m_limit(move_if<std::is_constructible_v<Limit, C1&&>>(
          std::move(c1), std::nullopt)) {}

  template<typename C1, typename C2>
  SelectClause::SelectClause(std::vector<std::string> columns,
      FromClause from, C1&& c1, C2&& c2)
      : m_columns(std::move(columns)),
        m_from(std::move(from)),
        m_where(move_if<std::is_constructible_v<Expression, C1&&>,
          std::is_constructible_v<Expression, C2&&>>(std::move(c1),
          std::move(c2), std::nullopt)),
        m_order(move_if<std::is_constructible_v<Order, C1&&>,
          std::is_constructible_v<Order, C2&&>>(std::move(c1), std::move(c2),
          std::nullopt)),
        m_limit(move_if<std::is_constructible_v<Limit, C1&&>,
          std::is_constructible_v<Limit, C2&&>>(std::move(c1), std::move(c2),
          std::nullopt)) {}

  template<typename C1, typename C2, typename C3>
  SelectClause::SelectClause(std::vector<std::string> columns,
      FromClause from, C1&& c1, C2&& c2, C3&& c3)
      : m_columns(std::move(columns)),
        m_from(std::move(from)),
        m_where(move_if<std::is_constructible_v<Expression, C1&&>,
          std::is_constructible_v<Expression, C2&&>,
          std::is_constructible_v<Expression, C3&&>>(std::move(c1),
          std::move(c2), std::move(c3), std::nullopt)),
        m_order(move_if<std::is_constructible_v<Order, C1&&>,
          std::is_constructible_v<Order, C2&&>,
          std::is_constructible_v<Order, C3&&>>(std::move(c1), std::move(c2),
          std::move(c3), std::nullopt)),
        m_limit(move_if<std::is_constructible_v<Limit, C1&&>,
          std::is_constructible_v<Limit, C2&&>,
          std::is_constructible_v<Limit, C3&&>>(std::move(c1), std::move(c2),
          std::move(c3), std::nullopt)) {}

  inline const std::vector<std::string>& SelectClause::get_columns() const {
    return m_columns;
  }

  inline const FromClause& SelectClause::get_from() const {
    return m_from;
  }

  inline const std::optional<Expression>& SelectClause::get_where() const {
    return m_where;
  }

  inline const std::optional<Order>& SelectClause::get_order() const {
    return m_order;
  }

  inline const std::optional<Limit>& SelectClause::get_limit() const {
    return m_limit;
  }
}

#endif
