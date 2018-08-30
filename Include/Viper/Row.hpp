#ifndef VIPER_ROW_HPP
#define VIPER_ROW_HPP
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

  /** Trait that tests if a type is a getter callable. */
  template<typename F, typename T>
  struct is_getter : std::integral_constant<bool,
    std::is_invocable_v<F, const T&>> {};

  /** Trait that tests if a type is a getter callable. */
  template<typename F, typename T>
  constexpr auto is_getter_v = is_getter<F, T>::value;

  /** Trait to deduce the return type of a getter, or void if parameter is not
      a getter callable. */
  template<typename F, typename T, bool Enabled>
  struct getter_result;

  template<typename F, typename T>
  struct getter_result<F, T, false> {
    using type = void;
  };

  template<typename F, typename T>
  struct getter_result<F, T, true> {
    using type = std::decay_t<std::invoke_result_t<F, T>>;
  };

  /** Trait to deduce the return type of a getter, or void if parameter is not
      a getter callable. */
  template<typename F, typename T>
  using getter_result_t = typename getter_result<F, T,
    is_getter_v<F, T>>::type;

  /** Trait that tests if a type is a setter callable. */
  template<typename F, typename T, typename V>
  struct is_setter : std::integral_constant<bool,
    std::is_invocable_v<F, const T&, V>> {};

  /** Trait that tests if a type is a setter callable. */
  template<typename F, typename T, typename V>
  constexpr auto is_setter_v = is_setter<F, T, V>::value;

  /** Trait that tests if a type is both a getter and a setter callable. */
  template<typename F, typename T>
  struct is_accessor : std::integral_constant<bool,
    is_getter_v<F, T> && is_setter_v<F, T, getter_result_t<F, T>>> {};

  /** Trait that tests if a type is both a getter and a setter callable. */
  template<typename F, typename T>
  constexpr auto is_accessor_v = is_accessor<F, T>::value;

  /*! \brief Defines a row in an SQL table.
      \tparam T The type used to represent a row.
   */
  template<typename T>
  class Row {
    public:

      //! The type used to represent a row.
      using Type = T;

      //! Constructs an empty row.
      Row();

      //! Returns the list of columns.
      const std::vector<Column>& get_columns() const;

      //! Returns the list of indexes (including the primary key).
      const std::vector<Index>& get_indexes() const;

      //! Extracts an SQL row.
      /*!
        \param row The SQL row of values to extract.
        \param value The value to store the rows in.
      */
      void extract(const char** row, Type& value) const;

      //! Appends a value to an SQL query string.
      /*!
        \param value The value to append.
        \param column The index of the column to append.
        \param query The query string to append the value to.
      */
      void append_value(const Type& value, int column,
        std::string& query) const;

      //! Appends a column.
      /*!
        \param name The name of the column.
        \return A new row containing the column.
      */
      Row add_column(std::string name) const;

      //! Appends a column using getter and setter methods.
      /*!
        \param name The name of the column.
        \param getter The method used to return the value to store.
        \param setter The method used to set the value retrieved.
        \return A new row containing the column.
      */
      template<typename G, typename S>
      std::enable_if_t<is_getter_v<G, Type>, Row> add_column(std::string name,
        G getter, S setter) const;

      //! Appends a column using getter and setter methods.
      /*!
        \param name The name of the column.
        \param t The column's data type.
        \param getter The method used to return the value to store.
        \param setter The method used to set the value retrieved.
        \return A new row containing the column.
      */
      template<typename G, typename S>
      std::enable_if_t<is_getter_v<G, Type>, Row> add_column(std::string name,
        const DataType& t, G getter, S setter) const;

      //! Appends a column tied directly to a data member.
      /*!
        \param name The name of the column.
        \param member A pointer to the member to tie the column to.
        \return A new row containing the column.
      */
      template<typename U, typename V = T>
      std::enable_if_t<std::is_class_v<V>, Row<V>> add_column(std::string name,
        U V::* member) const;

      //! Appends a column tied directly to a data member.
      /*!
        \param name The name of the column.
        \param t The column's data type.
        \param member A pointer to the member to tie the column to.
        \return A new row containing the column.
      */
      template<typename U, typename V = T>
      std::enable_if_t<std::is_class_v<V>, Row<V>> add_column(std::string name,
        const DataType& t, U V::* member) const;

      //! Appends a column tied to a function used to access the column.
      /*!
        \param name The name of the column.
        \param accessor The function used to get a reference to the member.
        \return A new row containing the column.
      */
      template<typename F>
      std::enable_if_t<!is_accessor_v<F, Type>, Row> add_column(
        std::string name, F accessor) const;

      //! Appends a column tied to a function used to access the column.
      /*!
        \param name The name of the column.
        \param t The column's data type.
        \param accessor The function used to get a reference to the member.
        \return A new row containing the column.
      */
      template<typename F>
      std::enable_if_t<!is_accessor_v<F, Type>, Row> add_column(
        std::string name, const DataType& t, F accessor) const;

      //! Appends a column tied to a function used to access the column.
      /*!
        \param name The name of the column.
        \param accessor The function used to get a reference to the member.
        \return A new row containing the column.
      */
      template<typename F>
      std::enable_if_t<is_accessor_v<F, Type>, Row> add_column(std::string name,
        F accessor) const;

      //! Appends a column tied to a function used to access the column.
      /*!
        \param name The name of the column.
        \param t The column's data type.
        \param accessor The function used to get a reference to the member.
        \return A new row containing the column.
      */
      template<typename F>
      std::enable_if_t<is_accessor_v<F, Type>, Row> add_column(std::string name,
        const DataType& t, F accessor) const;

      //! Sets the row's primary key.
      /*!
        \param columns A column to use as the primary key.
        \return A new row containing the primary key.
      */
      Row set_primary_key(std::string column) const;

      //! Sets the row's primary key.
      /*!
        \param columns A list of column names to use as the primary key.
        \return A new row containing the primary key.
      */
      Row set_primary_key(std::initializer_list<std::string> columns) const;

      //! Sets the row's primary key.
      /*!
        \param columns A list of column names to use as the primary key.
        \return A new row containing the primary key.
      */
      Row set_primary_key(std::vector<std::string> columns) const;

      //! Adds an index.
      /*!
        \param name The name of the index.
        \param column The column to use as an index.
        \return A new row containing the index.
      */
      Row add_index(std::string name, std::string column) const;

      //! Adds an index.
      /*!
        \param name The name of the index.
        \param columns A list of column names to use as an index.
        \return A new row containing the index.
      */
      Row add_index(std::string name,
        std::initializer_list<std::string> columns) const;

      //! Adds an index.
      /*!
        \param name The name of the index.
        \param columns A list of column names to use as an index.
        \return A new row containing the index.
      */
      Row add_index(std::string name, std::vector<std::string> columns) const;

    private:
      struct Accessors {
        std::function<void (const Type& value, std::string& columns)> m_getter;
        std::function<void (Type& value, const char** columns)> m_setter;
        int m_count;

        Accessors(std::function<void (const Type& value, std::string& columns)>
          getter, std::function<void (Type& value, const char** columns)>
          setter, int count);
      };
      struct Data {
        std::vector<Column> m_columns;
        std::vector<Accessors> m_accessors;
        std::vector<Index> m_indexes;
      };
      std::shared_ptr<Data> m_data;

      Row clone() const;
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

  //! Makes a getter function from a lambda.
  template<typename T, typename F>
  auto make_getter(F&& getter) {
    return make_getter(
      [getter = std::forward<F>(getter)] (const T& value) -> decltype(auto) {
        return getter(value);
      });
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

  //! Makes a setter function from a lambda.
  template<typename T, typename C, typename F>
  auto make_setter(F&& setter) {
    return make_setter(
      [setter = std::forward<F>(setter)] (T& value, C column) {
        setter(value, std::move(column));
      });
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
  Row<T>::Accessors::Accessors(
      std::function<void (const Type& value, std::string& columns)> getter,
      std::function<void (Type& value, const char** columns)> setter, int count)
      : m_getter(std::move(getter)),
        m_setter(std::move(setter)),
        m_count(count) {}

  template<typename T>
  Row<T>::Row()
      : m_data(std::make_shared<Data>()) {}

  template<typename T>
  const std::vector<Column>& Row<T>::get_columns() const {
    return m_data->m_columns;
  }

  template<typename T>
  const std::vector<Index>& Row<T>::get_indexes() const {
    return m_data->m_indexes;
  }

  template<typename T>
  void Row<T>::extract(const char** row, Type& value) const {
    for(auto& accessor : m_data->m_accessors) {
      accessor.m_setter(value, row);
      ++row;
    }
  }

  template<typename T>
  void Row<T>::append_value(const Type& value, int column,
      std::string& query) const {
    m_data->m_accessors[column].m_getter(value, query);
  }

  template<typename T>
  Row<T> Row<T>::add_column(std::string name) const {
    return add_column(std::move(name),
      std::function<const Type& (const Type&)>(
        [] (const Type& v) -> decltype(auto) {
          return v;
        }),
      std::function<void (Type&, Type)>(
        [] (Type& v, Type s) {
          v = std::move(s);
        }));
  }

  template<typename T>
  template<typename G, typename S>
  std::enable_if_t<is_getter_v<G, T>, Row<T>> Row<T>::add_column(
      std::string name, G getter, S setter) const {
    return add_column(std::move(name), native_to_data_type_v<Column>,
      make_getter<Type>(std::move(getter)),
      make_setter<Type, getter_result_t<G, T>>(std::move(setter)));
  }

  template<typename T>
  template<typename G, typename S>
  std::enable_if_t<is_getter_v<G, T>, Row<T>> Row<T>::add_column(
      std::string name, const DataType& t, G getter, S setter) const {
    auto r = clone();
    r.m_data->m_columns.emplace_back(std::move(name), t, false);
    r.m_data->m_accessors.emplace_back(
      [getter = make_getter<Type>(std::forward<G>(getter))] (
          const Type& value, std::string& columns) {
        to_sql(getter(value), columns);
      },
      [setter =
          make_setter<Type, getter_result_t<G, Type>>(
          std::forward<S>(setter))] (Type& value, const char** columns) {
        setter(value, from_sql<get_argument_t<S>>(columns[0]));
      },
      1);
    return r;
  }

  template<typename T>
  template<typename U, typename V>
  std::enable_if_t<std::is_class_v<V>, Row<V>> Row<T>::add_column(
      std::string name, U V::* member) const {
    return add_column(std::move(name), native_to_data_type_v<U>, member);
  }

  template<typename T>
  template<typename U, typename V>
  std::enable_if_t<std::is_class_v<V>, Row<V>> Row<T>::add_column(
      std::string name, const DataType& t, U V::* member) const {
    return add_column(std::move(name), t, make_getter(member),
      make_setter(member));
  }

  template<typename T>
  template<typename F>
  std::enable_if_t<!is_accessor_v<F, T>, Row<T>> Row<T>::add_column(
      std::string name, F accessor) const {
    return add_column(std::move(name),
      [=] (auto& value) -> decltype(auto) {
        return accessor(value);
      },
      [=] (auto& value, auto&& column) {
        accessor(value) = std::forward<decltype(column)>(column);
      });
  }

  template<typename T>
  template<typename F>
  std::enable_if_t<!is_accessor_v<F, T>, Row<T>> Row<T>::add_column(
      std::string name, const DataType& t, F accessor) const {
    return add_column(std::move(name), t,
      [=] (auto& value) -> decltype(auto) {
        return accessor(value);
      },
      [=] (auto& value, auto& column) {
        accessor(value) = column;
      });
  }

  template<typename T>
  template<typename F>
  std::enable_if_t<is_accessor_v<F, T>, Row<T>> Row<T>::add_column(
      std::string name, F accessor) const {
    return add_column(name, accessor, accessor);
  }

  template<typename T>
  template<typename F>
  std::enable_if_t<is_accessor_v<F, T>, Row<T>> Row<T>::add_column(
      std::string name, const DataType& t, F accessor) const {
    return add_column(name, t, accessor, accessor);
  }

  template<typename T>
  Row<T> Row<T>::set_primary_key(std::string column) const {
    return set_primary_key({column});
  }

  template<typename T>
  Row<T> Row<T>::set_primary_key(
      std::initializer_list<std::string> columns) const {
    return set_primary_key(std::vector<std::string>{columns});
  }

  template<typename T>
  Row<T> Row<T>::set_primary_key(std::vector<std::string> columns) const {
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
  Row<T> Row<T>::add_index(std::string name, std::string column) const {
    return add_index(std::move(name), {column});
  }

  template<typename T>
  Row<T> Row<T>::add_index(std::string name,
      std::initializer_list<std::string> columns) const {
    return add_index(std::move(name), std::vector<std::string>{columns});
  }

  template<typename T>
  Row<T> Row<T>::add_index(std::string name,
      std::vector<std::string> columns) const {
    Index i;
    i.m_name = std::move(name);
    i.m_columns = std::move(columns);
    auto r = clone();
    r.m_data->m_indexes.push_back(std::move(i));
    return r;
  }

  template<typename T>
  Row<T> Row<T>::clone() const {
    Row r;
    *r.m_data = *m_data;
    return r;
  }
}

#endif
