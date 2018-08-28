#ifndef VIPER_CONVERSIONS_HPP
#define VIPER_CONVERSIONS_HPP
#include <cstdint>
#include <string>
#include <string_view>
#include "viper/utilities.hpp"

namespace Viper {

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
  struct to_sql<double> {
    void operator ()(double value, std::string& column) const {
      column += std::to_string(value);
    }
  };

  template<>
  struct from_sql<double> {
    double operator ()(const char* column) const {
      return std::stod(column);
    }
  };

  template<>
  struct to_sql<float> {
    void operator ()(float value, std::string& column) const {
      column += std::to_string(value);
    }
  };

  template<>
  struct from_sql<float> {
    float operator ()(const char* column) const {
      return std::stof(column);
    }
  };

  template<>
  struct to_sql<std::int32_t> {
    void operator ()(std::int32_t value, std::string& column) const {
      column += std::to_string(value);
    }
  };

  template<>
  struct from_sql<std::int32_t> {
    std::int32_t operator ()(const char* column) const {
      return std::stoi(column);
    }
  };

  template<>
  struct to_sql<std::string> {
    void operator ()(std::string value, std::string& column) const {
      escape(value, column);
    }
  };

  template<>
  struct from_sql<std::string> {
    std::string operator ()(const char* column) const {
      return column;
    }
  };

  template<std::size_t N>
  struct to_sql<char[N]> {
    void operator ()(const char (&value)[N], std::string& column) const {
      escape(value, column);
    }
  };
}

#endif
