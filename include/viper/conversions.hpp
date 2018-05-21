#ifndef VIPER_CONVERSIONS_HPP
#define VIPER_CONVERSIONS_HPP
#include <string>

namespace viper {

  /*! \brief Callable data type used to convert a value to an SQL column.
      \tparam T The data type to convert.
   */
  template<typename T, typename = void>
  struct to_sql {};

  //! Converts a value to an SQL column.
  /*!
    \param value The value to convert.
    \param column The column to store the value in.
  */
  template<typename T>
  void convert_to_sql(const T& value, std::string& column) {
    to_sql<T>()(value, column);
  }

  /*! \brief Callable data type used to convert SQL columns to a value.
      \tparam T The data type to convert.
   */
  template<typename T, typename = void>
  struct from_sql {};

  //! Converts a list of SQL columns to a value.
  /*!
    \param columns The columns to convert.
  */
  template<typename T>
  T convert_from_sql(const char** columns) {
    return from_sql<T>()(columns);
  }

  //! Converts an SQL column to a value.
  /*!
    \param column The column to convert.
  */
  template<typename T>
  T convert_from_sql(const char* column) {
    return from_sql<T>()(column);
  }

  template<>
  struct to_sql<int> {
    void operator ()(int value, std::string& column) const {
      column += std::to_string(value);
    }
  };

  template<>
  struct from_sql<int> {
    int operator ()(const char* column) const {
      return std::stoi(column);
    }
  };
}

#endif
