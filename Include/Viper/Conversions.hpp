#ifndef VIPER_CONVERSIONS_HPP
#define VIPER_CONVERSIONS_HPP
#include <cstdint>
#include <string>
#include <string_view>
#include "Viper/Utilities.hpp"

namespace Viper {

  /*! \brief Callable data type used to convert a value to an SQL column.
      \tparam T The data type to convert.
   */
  template<typename T, typename = void>
  struct ToSql {};

  //! Converts a value to an SQL column.
  /*!
    \param value The value to convert.
    \param column The column to store the value in.
  */
  template<typename T>
  void to_sql(const T& value, std::string& column) {
    ToSql<T>()(value, column);
  }

  /*! \brief Callable data type used to convert SQL columns to a value.
      \tparam T The data type to convert.
   */
  template<typename T, typename = void>
  struct FromSql {};

  //! Converts a list of SQL columns to a value.
  /*!
    \param columns The columns to convert.
  */
  template<typename T>
  T from_sql(const char** columns) {
    return FromSql<T>()(columns);
  }

  //! Converts an SQL column to a value.
  /*!
    \param column The column to convert.
  */
  template<typename T>
  T from_sql(const char* column) {
    return FromSql<T>()(column);
  }

  template<>
  struct ToSql<double> {
    void operator ()(double value, std::string& column) const {
      column += std::to_string(value);
    }
  };

  template<>
  struct FromSql<double> {
    double operator ()(const char* column) const {
      return std::stod(column);
    }
  };

  template<>
  struct ToSql<float> {
    void operator ()(float value, std::string& column) const {
      column += std::to_string(value);
    }
  };

  template<>
  struct FromSql<float> {
    float operator ()(const char* column) const {
      return std::stof(column);
    }
  };

  template<>
  struct ToSql<std::int32_t> {
    void operator ()(std::int32_t value, std::string& column) const {
      column += std::to_string(value);
    }
  };

  template<>
  struct FromSql<std::int32_t> {
    std::int32_t operator ()(const char* column) const {
      return std::stoi(column);
    }
  };

  template<>
  struct ToSql<std::string> {
    void operator ()(std::string value, std::string& column) const {
      escape(value, column);
    }
  };

  template<>
  struct FromSql<std::string> {
    std::string operator ()(const char* column) const {
      return column;
    }
  };

  template<std::size_t N>
  struct ToSql<char[N]> {
    void operator ()(const char (&value)[N], std::string& column) const {
      escape(value, column);
    }
  };
}

#endif
