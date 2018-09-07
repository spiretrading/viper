#ifndef VIPER_CONVERSIONS_HPP
#define VIPER_CONVERSIONS_HPP
#include <cstdint>
#include <optional>
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
  auto from_sql(const char** columns) {
    return FromSql<T>()(columns);
  }

  //! Converts an SQL column to a value.
  /*!
    \param column The column to convert.
  */
  template<typename T>
  auto from_sql(const char* column) {
    return FromSql<T>()(column);
  }

  template<>
  struct ToSql<bool> {
    void operator ()(bool value, std::string& column) const {
      if(value) {
        column += '1';
      } else {
        column += '0';
      }
    }
  };

  template<>
  struct FromSql<bool> {
    auto operator ()(const char* column) const {
      if(column[0] == '0') {
        return false;
      }
      return true;
    }
  };

  template<>
  struct ToSql<char> {
    void operator ()(char value, std::string& column) const {
      column += value;
    }
  };

  template<>
  struct FromSql<char> {
    auto operator ()(const char* column) const {
      return column[0];
    }
  };

  template<>
  struct ToSql<double> {
    void operator ()(double value, std::string& column) const {
      column += std::to_string(value);
    }
  };

  template<>
  struct FromSql<double> {
    auto operator ()(const char* column) const {
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
    auto operator ()(const char* column) const {
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
    auto operator ()(const char* column) const {
      return std::stoi(column);
    }
  };

  template<>
  struct ToSql<std::uint32_t> {
    void operator ()(std::uint32_t value, std::string& column) const {
      column += std::to_string(value);
    }
  };

  template<>
  struct FromSql<std::uint32_t> {
    auto operator ()(const char* column) const {
      return std::stoul(column);
    }
  };

  template<>
  struct ToSql<std::int64_t> {
    void operator ()(std::int64_t value, std::string& column) const {
      column += std::to_string(value);
    }
  };

  template<>
  struct FromSql<std::int64_t> {
    auto operator ()(const char* column) const {
      return std::stoll(column);
    }
  };

  template<>
  struct ToSql<std::uint64_t> {
    void operator ()(std::uint64_t value, std::string& column) const {
      column += std::to_string(value);
    }
  };

  template<>
  struct FromSql<std::uint64_t> {
    auto operator ()(const char* column) const {
      return std::stoull(column);
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
    auto operator ()(const char* column) const {
      return std::string(column);
    }
  };

  template<std::size_t N>
  struct ToSql<char[N]> {
    void operator ()(const char (&value)[N], std::string& column) const {
      escape(value, column);
    }
  };

  template<typename T>
  struct ToSql<std::optional<T>> {
    void operator ()(const std::optional<T>& value, std::string& column) const {
      if(value.has_value()) {
        to_sql(*value, column);
      } else {
        column += "NULL";
      }
    }
  };

  template<typename T>
  struct FromSql<std::optional<T>> {
    std::optional<T> operator ()(const char* column) const {
      if(column == nullptr) {
        return std::nullopt;
      }
      return from_sql<T>(column);
    }
  };
}

#endif
