#ifndef VIPER_SELECT_STATEMENT_HPP
#define VIPER_SELECT_STATEMENT_HPP
#include <optional>
#include <string>
#include <type_traits>
#include "viper/expressions/expression.hpp"
#include "viper/table.hpp"
#include "viper/utilities.hpp"

namespace viper {

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

      //! Constructs a select statement with a single optional clause.
      /*!
        \param r The result table.
        \param from The table to select from.
        \param c1 The first clause to include.
        \param first An output iterator used to store the rows.
      */
      template<typename C1>
      select_statement(result_table r, std::string from, C1&& c1,
        destination first);

      //! Constructs a select statement with a two optional clauses.
      /*!
        \param r The result table.
        \param from The table to select from.
        \param c1 The first clause to include.
        \param c2 The second clause to include.
        \param first An output iterator used to store the rows.
      */
      template<typename C1, typename C2>
      select_statement(result_table r, std::string from, C1&& c1, C2&& c2,
        destination first);

      //! Constructs a select statement with a three optional clauses.
      /*!
        \param r The result table.
        \param from The table to select from.
        \param c1 The first clause to include.
        \param c2 The second clause to include.
        \param c3 The third clause to include.
        \param first An output iterator used to store the rows.
      */
      template<typename C1, typename C2, typename C3>
      select_statement(result_table r, std::string from, C1&& c1, C2&& c2,
        C3&& c3, destination first);

      //! Returns the result table.
      const result_table& get_result_table() const;

      //! Returns the table to select from.
      const std::string& get_from_table() const;

      //! Returns the where clause.
      const std::optional<expression>& get_where() const;

      //! Returns the order by clause.
      const std::optional<order>& get_order() const;

      //! Returns the limit.
      const std::optional<limit>& get_limit() const;

      //! Returns the destination iterator.
      destination get_first() const;

    private:
      result_table m_result_table;
      std::string m_from_table;
      std::optional<expression> m_where;
      std::optional<order> m_order;
      std::optional<limit> m_limit;
      destination m_first;

      select_statement(result_table r, std::string from,
        std::optional<expression> where, std::optional<order> o,
        std::optional<limit> l, destination first);
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

  //! Builds a select statement with a single clause.
  /*!
    \param r The result table.
    \param from The table to select from.
    \param c1 The first clause.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D, typename C1>
  auto select(T r, std::string from, C1&& c1, D first) {
    return select_statement(std::move(r), std::move(from), std::move(c1),
      std::move(first));
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
  auto select(T r, std::string from, C1&& c1, C2&& c2, D first) {
    return select_statement(std::move(r), std::move(from), std::move(c1),
      std::move(c2), std::move(first));
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
  auto select(T r, std::string from, C1&& c1, C2&& c2, C3&& c3, D first) {
    return select_statement(std::move(r), std::move(from), std::move(c1),
      std::move(c2), std::move(c3), std::move(first));
  }

  inline limit::limit(int value)
      : m_value(value) {}

  inline order::order(std::vector<std::string> columns, int o)
      : m_columns(std::move(columns)),
        m_order(o) {}

  template<typename T, typename D>
  select_statement<T, D>::select_statement(result_table r, std::string from,
      destination first)
      : m_result_table(std::move(r)),
        m_from_table(std::move(from)),
        m_first(std::move(first)) {}

  template<typename T, typename D>
  template<typename C1>
  select_statement<T, D>::select_statement(result_table r, std::string from,
      C1&& c1, destination first)
      : m_result_table(std::move(r)),
        m_from_table(std::move(from)),
        m_where(move_if<std::is_constructible_v<expression, C1&&>>(
          std::move(c1), std::nullopt)),
        m_order(move_if<std::is_constructible_v<order, C1&&>>(
          std::move(c1), std::nullopt)),
        m_limit(move_if<std::is_constructible_v<limit, C1&&>>(
          std::move(c1), std::nullopt)),
        m_first(std::move(first)) {}

  template<typename T, typename D>
  template<typename C1, typename C2>
  select_statement<T, D>::select_statement(result_table r, std::string from,
      C1&& c1, C2&& c2, destination first)
      : m_result_table(std::move(r)),
        m_from_table(std::move(from)),
        m_where(move_if<std::is_constructible_v<expression, C1&&>,
          std::is_constructible_v<expression, C2&&>>(std::move(c1),
          std::move(c2), std::nullopt)),
        m_order(move_if<std::is_constructible_v<order, C1&&>,
          std::is_constructible_v<order, C2&&>>(std::move(c1), std::move(c2),
          std::nullopt)),
        m_limit(move_if<std::is_constructible_v<limit, C1&&>,
          std::is_constructible_v<limit, C2&&>>(std::move(c1), std::move(c2),
          std::nullopt)),
        m_first(std::move(first)) {}

  template<typename T, typename D>
  template<typename C1, typename C2, typename C3>
  select_statement<T, D>::select_statement(result_table r, std::string from,
      C1&& c1, C2&& c2, C3&& c3, destination first)
      : m_result_table(std::move(r)),
        m_from_table(std::move(from)),
        m_where(move_if<std::is_constructible_v<expression, C1&&>,
          std::is_constructible_v<expression, C2&&>,
          std::is_constructible_v<expression, C3&&>>(std::move(c1),
          std::move(c2), std::move(c3), std::nullopt)),
        m_order(move_if<std::is_constructible_v<order, C1&&>,
          std::is_constructible_v<order, C2&&>,
          std::is_constructible_v<order, C3&&>>(std::move(c1), std::move(c2),
          std::move(c3), std::nullopt)),
        m_limit(move_if<std::is_constructible_v<limit, C1&&>,
          std::is_constructible_v<limit, C2&&>,
          std::is_constructible_v<limit, C3&&>>(std::move(c1), std::move(c2),
          std::move(c3), std::nullopt)),
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
  const std::optional<order>& select_statement<T, D>::get_order() const {
    return m_order;
  }

  template<typename T, typename D>
  const std::optional<limit>& select_statement<T, D>::get_limit() const {
    return m_limit;
  }

  template<typename T, typename D>
  typename select_statement<T, D>::destination
      select_statement<T, D>::get_first() const {
    return m_first;
  }

  template<typename T, typename D>
  select_statement<T, D>::select_statement(result_table r, std::string from,
      std::optional<expression> where, std::optional<order> o,
      std::optional<limit> l, destination first)
      : m_result_table(std::move(r)),
        m_from_table(std::move(from)),
        m_where(std::move(where)),
        m_order(std::move(o)),
        m_limit(std::move(l)),
        m_first(std::move(first)) {}
}

#endif
