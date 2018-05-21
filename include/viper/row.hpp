#ifndef VIPER_ROW_HPP
#define VIPER_ROW_HPP
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include "viper/conversions.hpp"
#include "viper/function_traits.hpp"

namespace viper {

  /*! \brief Defines a single row stored in an SQL table.
      \tparam T The type to store the row in.
   */
  template<typename T>
  class row {
    public:

      //! The type to store the row in.
      using type = T;

      //! Constructs an empty row.
      row();

      //! Returns the names of the columns.
      const std::vector<std::string>& get_names() const;

      //! Defines a column.
      /*!
        \param name The name of the column.
        \param getter The function used to return the value to store.
        \param setter The function used to set the value retrived.
        \return A new row containing the column.
      */
      template<typename G, typename S>
      row add_column(std::string_view name,
        std::function<G (const type& value)> getter,
        std::function<void (type& value, S column)> setter) const;

      //! Defines a column using getter and setter methods.
      /*!
        \param name The name of the column.
        \param getter The method used to return the value to store.
        \param setter The method used to set the value retrieved.
        \return A new row containing the column.
      */
      template<typename G, typename S>
      row add_column(std::string_view name, G getter, S setter) const;

      //! Defines a column tied directly to a data member.
      /*!
        \param name The name of the column.
        \param member A pointer to the member to tie the column to.
        \return A new row containing the column.
      */
      template<typename U>
      row add_column(std::string_view name, U type::* member) const;

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
        std::vector<std::string> m_names;
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
    return std::make_function(std::forward<F>(getter));
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
  row<T>::accessors::accessors(
      std::function<void (const type& value, std::string& columns)> getter,
      std::function<void (type& value, const char** columns)> setter, int count)
      : m_getter(std::move(getter)),
        m_setter(std::move(setter)),
        m_count(count) {}

  template<typename T>
  row<T>::row()
      : m_data(std::make_shared<data>()) {}

  template<typename T>
  const std::vector<std::string>& row<T>::get_names() const {
    return m_data->m_names;
  }

  template<typename T>
  template<typename G, typename S>
  row<T> row<T>::add_column(std::string_view name,
      std::function<G (const type& value)> getter,
      std::function<void (type& value, S column)> setter) const {
    row<T> r;
    r.m_data->m_names = m_data->m_names;
    r.m_data->m_accessors = m_data->m_accessors;
    r.m_data->m_names.emplace_back(name);
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
  row<T> row<T>::add_column(std::string_view name, G getter, S setter) const {
    return add_column(name, make_getter<T>(std::move(getter)),
      make_setter<T>(std::move(setter)));
  }

  template<typename T>
  template<typename U>
  row<T> row<T>::add_column(std::string_view name, U T::* member) const {
    return add_column(name,
      [=] (const type& value) -> const U& {
        return value.*member;
      },
      [=] (type& value, U column) {
        value.*member = std::move(column);
      });
  }
}

#endif
