#ifndef VIPER_TABLE_HPP
#define VIPER_TABLE_HPP
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>
#include "Viper/Column.hpp"
#include "Viper/Conversions.hpp"
#include "Viper/FunctionTraits.hpp"
#include "Viper/Index.hpp"
#include "Viper/DataTypes/NativeToDataType.hpp"

namespace Viper {

  /*! \brief Defines an SQL table.
      \tparam T The type used to represent a table row.
   */
  template<typename T>
  class Table {
    public:

      //! The type used to represent a table row.
      using type = T;

      //! Constructs an empty table.
      Table();

      //! Returns the list of columns.
      const std::vector<Column>& get_columns() const;

      //! Returns the list of indexes (including the primary key).
      const std::vector<Index>& get_indexes() const;

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

      //! Defines a column for a table with a single column.
      /*!
        \param name The name of the column.
        \return A new table containing the column.
      */
      Table add_column(std::string name) const;

      //! Defines a column using getter and setter methods.
      /*!
        \param name The name of the column.
        \param getter The method used to return the value to store.
        \param setter The method used to set the value retrieved.
        \return A new table containing the column.
      */
      template<typename G, typename S>
      std::enable_if_t<std::is_invocable_v<G, const type&>, Table> add_column(
        std::string name, G getter, S setter) const;

      //! Defines a column using getter and setter methods.
      /*!
        \param name The name of the column.
        \param t The column's data type.
        \param getter The method used to return the value to store.
        \param setter The method used to set the value retrieved.
        \return A new table containing the column.
      */
      template<typename G, typename S>
      std::enable_if_t<std::is_invocable_v<G, const type&>, Table> add_column(
        std::string name, const DataType& t, G getter, S setter) const;

      //! Defines a column tied directly to a data member.
      /*!
        \param name The name of the column.
        \param member A pointer to the member to tie the column to.
        \return A new table containing the column.
      */
      template<typename U, typename V = T>
      std::enable_if_t<std::is_class_v<V>, Table<V>> add_column(
        std::string name, U V::* member) const;

      //! Defines a column tied directly to a data member.
      /*!
        \param name The name of the column.
        \param t The column's data type.
        \param member A pointer to the member to tie the column to.
        \return A new table containing the column.
      */
      template<typename U, typename V = T>
      std::enable_if_t<std::is_class_v<V>, Table<V>> add_column(
        std::string name, const DataType& t, U V::* member) const;

      //! Sets the table's primary key.
      /*!
        \param columns A column to use as the primary key.
        \return A new table containing the primary key.
      */
      Table set_primary_key(std::string column) const;

      //! Sets the table's primary key.
      /*!
        \param columns A list of column names to use as the primary key.
        \return A new table containing the primary key.
      */
      Table set_primary_key(std::initializer_list<std::string> columns) const;

      //! Sets the table's primary key.
      /*!
        \param columns A list of column names to use as the primary key.
        \return A new table containing the primary key.
      */
      Table set_primary_key(std::vector<std::string> columns) const;

      //! Adds an index.
      /*!
        \param name The name of the index.
        \param column The column to use as an index.
        \return A new table containing the index.
      */
      Table add_index(std::string name, std::string column) const;

      //! Adds an index.
      /*!
        \param name The name of the index.
        \param columns A list of column names to use as an index.
        \return A new table containing the index.
      */
      Table add_index(std::string name,
        std::initializer_list<std::string> columns) const;

      //! Adds an index.
      /*!
        \param name The name of the index.
        \param columns A list of column names to use as an index.
        \return A new table containing the index.
      */
      Table add_index(std::string name, std::vector<std::string> columns) const;

    private:
      struct Accessors {
        std::function<void (const type& value, std::string& columns)> m_getter;
        std::function<void (type& value, const char** columns)> m_setter;
        int m_count;

        Accessors(std::function<void (const type& value, std::string& columns)>
          getter, std::function<void (type& value, const char** columns)>
          setter, int count);
      };
      struct Data {
        std::vector<Column> m_columns;
        std::vector<Accessors> m_accessors;
        std::vector<Index> m_indexes;
      };
      std::shared_ptr<Data> m_data;

      Table clone() const;
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
  template<typename F>
  auto make_getter(F&& getter) {
    return make_function(std::forward<F>(getter));
  }

  //! Makes a getter function from a pointer to member.
  template<typename T, typename U>
  auto make_getter(U T::* member) {
    return make_getter(
      [=] (const T& value) -> const U& {
        return value.*member;
      });
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
  template<typename F>
  auto make_setter(F&& setter) {
    return make_function(std::forward<F>(setter));
  }

  //! Makes a setter function from a pointer to member.
  template<typename T, typename U>
  auto make_setter(U T::* member) {
    return make_setter(
      [=] (T& value, U column) {
        value.*member = std::move(column);
      });
  }

  template<typename T>
  Table<T>::Accessors::Accessors(
      std::function<void (const type& value, std::string& columns)> getter,
      std::function<void (type& value, const char** columns)> setter, int count)
      : m_getter(std::move(getter)),
        m_setter(std::move(setter)),
        m_count(count) {}

  template<typename T>
  Table<T>::Table()
      : m_data(std::make_shared<Data>()) {}

  template<typename T>
  const std::vector<Column>& Table<T>::get_columns() const {
    return m_data->m_columns;
  }

  template<typename T>
  const std::vector<Index>& Table<T>::get_indexes() const {
    return m_data->m_indexes;
  }

  template<typename T>
  void Table<T>::extract(const char** row, type& value) const {
    for(auto& accessor : m_data->m_accessors) {
      accessor.m_setter(value, row);
      ++row;
    }
  }

  template<typename T>
  void Table<T>::append_value(const type& value, int column,
      std::string& query) const {
    m_data->m_accessors[column].m_getter(value, query);
  }

  template<typename T>
  Table<T> Table<T>::add_column(std::string name) const {
    return add_column(std::move(name),
      std::function<const type& (const type&)>(
        [] (const type& v) -> decltype(auto) {
          return v;
        }),
      std::function<void (type&, type)>(
        [] (type& v, type s) {
          v = std::move(s);
        }));
  }

  template<typename T>
  template<typename G, typename S>
  std::enable_if_t<std::is_invocable_v<G, const T&>, Table<T>>
      Table<T>::add_column(std::string name, G getter, S setter) const {
    return add_column(std::move(name), native_to_data_type_v<G>,
      make_getter<T>(std::move(getter)), make_setter<T>(std::move(setter)));
  }

  template<typename T>
  template<typename G, typename S>
  std::enable_if_t<std::is_invocable_v<G, const T&>, Table<T>> Table<T>::
      add_column(std::string name, const DataType& t, G getter,
      S setter) const {
    auto r = clone();
    r.m_data->m_columns.emplace_back(std::move(name), t, false);
    r.m_data->m_accessors.emplace_back(
      [getter = make_getter(std::forward<G>(getter))] (
          const type& value, std::string& columns) {
        to_sql(getter(value), columns);
      },
      [setter = make_setter(std::forward<S>(setter))] (
          type& value, const char** columns) {
        setter(value, from_sql<get_argument_t<S>>(columns[0]));
      },
      1);
    return r;
  }

  template<typename T>
  template<typename U, typename V>
  std::enable_if_t<std::is_class_v<V>, Table<V>> Table<T>::add_column(
      std::string name, U V::* member) const {
    return add_column(std::move(name), native_to_data_type_v<U>, member);
  }

  template<typename T>
  template<typename U, typename V>
  std::enable_if_t<std::is_class_v<V>, Table<V>> Table<T>::add_column(
      std::string name, const DataType& t, U V::* member) const {
    return add_column(std::move(name), t, make_getter(member),
      make_setter(member));
  }

  template<typename T>
  Table<T> Table<T>::set_primary_key(std::string column) const {
    return set_primary_key({column});
  }

  template<typename T>
  Table<T> Table<T>::set_primary_key(
      std::initializer_list<std::string> columns) const {
    return set_primary_key(std::vector<std::string>{columns});
  }

  template<typename T>
  Table<T> Table<T>::set_primary_key(std::vector<std::string> columns) const {
    Index i;
    i.m_columns = std::move(columns);
    i.m_is_primary = true;
    i.m_is_unique = true;
    auto r = clone();
    if(!r.m_data->m_indexes.empty()) {
      r.m_data->m_indexes.front().m_is_primary = false;
    }
    r.m_data->m_indexes.insert(r.m_data->m_indexes.begin(), std::move(i));
    return r;
  }

  template<typename T>
  Table<T> Table<T>::add_index(std::string name, std::string column) const {
    return add_index(std::move(name), {column});
  }

  template<typename T>
  Table<T> Table<T>::add_index(std::string name,
      std::initializer_list<std::string> columns) const {
    return add_index(std::move(name), std::vector<std::string>{columns});
  }

  template<typename T>
  Table<T> Table<T>::add_index(std::string name,
      std::vector<std::string> columns) const {
    Index i;
    i.m_name = std::move(name);
    i.m_columns = std::move(columns);
    auto r = clone();
    r.m_data->m_indexes.push_back(std::move(i));
    return r;
  }

  template<typename T>
  Table<T> Table<T>::clone() const {
    Table r;
    *r.m_data = *m_data;
    return r;
  }
}

#endif
