#ifndef VIPER_SELECT_STATEMENT_HPP
#define VIPER_SELECT_STATEMENT_HPP
#include <optional>
#include <string>
#include "viper/expressions/expression.hpp"
#include "viper/table.hpp"

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

      //! Constructs a select statement.
      /*!
        \param r The result table.
        \param from The table to select from.
        \param where The where clause.
        \param first An output iterator used to store the rows.
      */
      select_statement(result_table r, std::string from, expression where,
        destination first);

      //! Constructs a select statement.
      /*!
        \param r The result table.
        \param from The table to select from.
        \param o The ordering of the rows.
        \param first An output iterator used to store the rows.
      */
      select_statement(result_table r, std::string from, order o,
        destination first);

      //! Constructs a select statement.
      /*!
        \param r The result table.
        \param from The table to select from.
        \param l The maximum number of rows.
        \param first An output iterator used to store the rows.
      */
      select_statement(result_table r, std::string from, limit l,
        destination first);

      //! Constructs a select statement.
      /*!
        \param r The result table.
        \param from The table to select from.
        \param where The where clause.
        \param l The maximum number of rows.
        \param first An output iterator used to store the rows.
      */
      select_statement(result_table r, std::string from, expression where,
        limit l, destination first);

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

  //! Builds a select statement.
  /*!
    \param r The result table.
    \param from The table to select from.
    \param o The ordering of the rows.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D>
  auto select(T r, std::string from, order o, D first) {
    return select_statement(std::move(r), std::move(from), std::move(o),
      std::move(first));
  }

  //! Builds a select statement.
  /*!
    \param r The result table.
    \param from The table to select from.
    \param limit The maximum number of rows.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D>
  auto select(T r, std::string from, limit l, D first) {
    return select_statement(std::move(r), std::move(from), std::move(l),
      std::move(first));
  }

  //! Builds a select statement.
  /*!
    \param r The result table.
    \param from The table to select from.
    \param where The where clause.
    \param limit The maximum number of rows.
    \param first An output iterator used to store the rows.
  */
  template<typename T, typename D>
  auto select(T r, std::string from, expression where, limit l, D first) {
    return select_statement(std::move(r), std::move(from), std::move(where),
      std::move(l), std::move(first));
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
  select_statement<T, D>::select_statement(result_table r, std::string from,
      expression where, destination first)
      : m_result_table(std::move(r)),
        m_from_table(std::move(from)),
        m_where(std::move(where)),
        m_first(std::move(first)) {}

  template<typename T, typename D>
  select_statement<T, D>::select_statement(result_table r, std::string from,
      order o, destination first)
      : m_result_table(std::move(r)),
        m_from_table(std::move(from)),
        m_order(std::move(o)),
        m_first(std::move(first)) {}

  template<typename T, typename D>
  select_statement<T, D>::select_statement(result_table r, std::string from,
      limit l, destination first)
      : m_result_table(std::move(r)),
        m_from_table(std::move(from)),
        m_limit(std::move(l)),
        m_first(std::move(first)) {}

  template<typename T, typename D>
  select_statement<T, D>::select_statement(result_table r, std::string from,
      expression where, limit l, destination first)
      : m_result_table(std::move(r)),
        m_from_table(std::move(from)),
        m_where(std::move(where)),
        m_limit(std::move(l)),
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
}

#endif
