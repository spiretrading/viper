#ifndef VIPER_TABLE_HPP
#define VIPER_TABLE_HPP
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>
#include "viper/column.hpp"
#include "viper/conversions.hpp"
#include "viper/function_traits.hpp"
#include "viper/data_types/native_to_data_type.hpp"

namespace viper {

  /*! \brief Defines an SQL table.
      \tparam T The type used to represent a table row.
   */
  template<typename T>
  class table {
    public:

      //! The type used to represent a table row.
      using type = T;

      //! Constructs an empty table.
      table();

      //! Returns the list of columns.
      const std::vector<column>& get_columns() const;

      //! Extracts an SQL row.
      /*!
        \param row The SQL row of values to extract.
        \param value The value to store the rows in.
      */
      void extract(const char** row, type& value) const;

      //! Appends a value to an SQL query string.
      /*!
        \param value The value to append.
        \param column The index of the column to append.
        \param query The query string to append the value to.
      */
      void append_value(const type& value, int column,
        std::string& query) const;

      //! Defines a column.
      /*!
        \param name The name of the column.
        \param t The columns SQL data type.
        \param getter The function used to return the value to store.
        \param setter The function used to set the value retrived.
        \return A new table containing the column.
      */
      template<typename G, typename S>
      table add_column(std::string name, const data_type& t,
        std::function<G (const type& value)> getter,
        std::function<void (type& value, S column)> setter) const;

      //! Defines a column.
      /*!
        \param name The name of the column.
        \param getter The function used to return the value to store.
        \param setter The function used to set the value retrived.
        \return A new table containing the column.
      */
      template<typename G, typename S>
      table add_column(std::string name,
        std::function<G (const type& value)> getter,
        std::function<void (type& value, S column)> setter) const;

      //! Defines a column using getter and setter methods.
      /*!
        \param name The name of the column.
        \param getter The method used to return the value to store.
        \param setter The method used to set the value retrieved.
        \return A new table containing the column.
      */
      template<typename G, typename S>
      std::enable_if_t<std::is_invocable_v<G> && std::is_invocable_v<S>, table>
        add_column(std::string name, G getter, S setter) const;

      //! Defines a column using getter and setter methods.
      /*!
        \param name The name of the column.
        \param t The column's data type.
        \param getter The method used to return the value to store.
        \param setter The method used to set the value retrieved.
        \return A new table containing the column.
      */
      template<typename G, typename S>
      table add_column(std::string name, const data_type& t, G getter,
        S setter) const;

      //! Defines a column tied directly to a data member.
      /*!
        \param name The name of the column.
        \param member A pointer to the member to tie the column to.
        \return A new table containing the column.
      */
      template<typename U>
      table add_column(std::string name, U type::* member) const;

      //! Defines a column tied directly to a data member.
      /*!
        \param name The name of the column.
        \param t The column's data type.
        \param member A pointer to the member to tie the column to.
        \return A new table containing the column.
      */
      template<typename U>
      table add_column(std::string name, const data_type& t,
        U type::* member) const;

    private:
      struct accessors {
        std::function<void (const type& value, std::string& columns)> m_getter;
        std::function<void (type& value, const char** columns)> m_setter;
        int m_count;

        accessors(std::function<void (const type& value, std::string& columns)>
          getter, std::function<void (type& value, const char** columns)>
          setter, int count);
      };
      struct data {
        std::vector<column> m_columns;
        std::vector<accessors> m_accessors;
      };
      std::shared_ptr<data> m_data;
  };

  //! Makes a getter function from a class method.
  template<typename T, typename R>
  auto make_getter(R (T::* getter)() const) {
    return std::function<R (const T&)>(getter);
  }

  //! Makes a getter function from a function.
  template<typename T, typename R>
  auto make_getter(std::function<R (const T&)> getter) {
    return getter;
  }

  //! Makes a getter function from a lambda.
  template<typename T, typename F>
  auto make_getter(F&& getter) {
    return make_function(std::forward<F>(getter));
  }

  //! Makes a setter function from a class method.
  template<typename T, typename R, typename S>
  auto make_setter(R (T::* setter)(S)) {
    return std::function<void (T&, S)>(setter);
  }

  //! Makes a setter function from a function.
  template<typename T, typename R, typename S>
  auto make_setter(std::function<R (T&, S)> setter) {
    return std::function<void (T&, S)>(std::move(setter));
  }

  //! Makes a setter function from a lambda.
  template<typename T, typename F>
  auto make_setter(F&& setter) {
    return make_function(std::forward<F>(setter));
  }

  template<typename T>
  table<T>::accessors::accessors(
      std::function<void (const type& value, std::string& columns)> getter,
      std::function<void (type& value, const char** columns)> setter, int count)
      : m_getter(std::move(getter)),
        m_setter(std::move(setter)),
        m_count(count) {}

  template<typename T>
  table<T>::table()
      : m_data(std::make_shared<data>()) {}

  template<typename T>
  const std::vector<column>& table<T>::get_columns() const {
    return m_data->m_columns;
  }

  template<typename T>
  void table<T>::extract(const char** row, type& value) const {
    for(auto& accessor : m_data->m_accessors) {
      accessor.m_setter(value, row);
      ++row;
    }
  }

  template<typename T>
  void table<T>::append_value(const type& value, int column,
      std::string& query) const {
    m_data->m_accessors[column].m_getter(value, query);
  }

  template<typename T>
  template<typename G, typename S>
  table<T> table<T>::add_column(std::string name, const data_type& t,
      std::function<G (const type& value)> getter,
      std::function<void (type& value, S column)> setter) const {
    table<T> r;
    r.m_data->m_columns = m_data->m_columns;
    r.m_data->m_accessors = m_data->m_accessors;
    r.m_data->m_columns.emplace_back(std::move(name), t);
    r.m_data->m_accessors.emplace_back(
      [=] (const type& value, std::string& columns) {
        convert_to_sql(getter(value), columns);
      },
      [=] (type& value, const char** columns) {
        setter(value, convert_from_sql<S>(columns[0]));
      },
      1);
    return r;
  }

  template<typename T>
  template<typename G, typename S>
  table<T> table<T>::add_column(std::string name,
      std::function<G (const type& value)> getter,
      std::function<void (type& value, S column)> setter) const {
    return add_column(std::move(name), native_to_data_type_v<G>,
      std::move(getter), std::move(setter));
  }

  template<typename T>
  template<typename G, typename S>
  std::enable_if_t<std::is_invocable_v<G> && std::is_invocable_v<S>, table<T>>
      table<T>::add_column(std::string name, G getter, S setter) const {
    return add_column(std::move(name), make_getter<T>(std::move(getter)),
      make_setter<T>(std::move(setter)));
  }

  template<typename T>
  template<typename G, typename S>
  table<T> table<T>::add_column(std::string name, const data_type& t, G getter,
      S setter) const {
    return add_column(std::move(name), t, make_getter<T>(std::move(getter)),
      make_setter<T>(std::move(setter)));
  }

  template<typename T>
  template<typename U>
  table<T> table<T>::add_column(std::string name, U T::* member) const {
    return add_column(std::move(name), native_to_data_type_v<U>, member);
  }

  template<typename T>
  template<typename U>
  table<T> table<T>::add_column(std::string name, const data_type& t,
      U type::* member) const {
    return add_column(std::move(name), t,
      [=] (const type& value) -> const U& {
        return value.*member;
      },
      [=] (type& value, U column) {
        value.*member = std::move(column);
      });
  }
}

#endif
