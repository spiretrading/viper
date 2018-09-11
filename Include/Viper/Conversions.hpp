#ifndef VIPER_CONVERSIONS_HPP
#define VIPER_CONVERSIONS_HPP
#include <cstdint>
#include <cstring>
#include <optional>
#include <string>
#include <string_view>
#include "Viper/DataTypes/BlobDataType.hpp"
#include "Viper/Utilities.hpp"

namespace Viper {

  //! Stores the column in raw bytes.
  struct RawColumn {

    //! The raw bytes encoding the value.
    const char* m_data;

    //! The size in bytes of the column.
    std::size_t m_size;
  };

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
  auto from_sql(const RawColumn* columns) {
    return FromSql<T>()(columns);
  }

  //! Converts an SQL column to a value.
  /*!
    \param column The column to convert.
  */
  template<typename T>
  auto from_sql(const RawColumn& column) {
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
    auto operator ()(const RawColumn& column) const {
      if(column.m_data[0] == '0') {
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
    auto operator ()(const RawColumn& column) const {
      return column.m_data[0];
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
    auto operator ()(const RawColumn& column) const {
      return std::stod(column.m_data);
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
    auto operator ()(const RawColumn& column) const {
      return std::stof(column.m_data);
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
    auto operator ()(const RawColumn& column) const {
      return std::stoi(column.m_data);
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
    auto operator ()(const RawColumn& column) const {
      return std::stoul(column.m_data);
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
    auto operator ()(const RawColumn& column) const {
      return std::stoll(column.m_data);
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
    auto operator ()(const RawColumn& column) const {
      return std::stoull(column.m_data);
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
    auto operator ()(const RawColumn& column) const {
      return std::string(column.m_data);
    }
  };

  template<std::size_t N>
  struct ToSql<char[N]> {
    void operator ()(const char (&value)[N], std::string& column) const {
      escape(value, column);
    }
  };

  template<>
  struct ToSql<std::vector<std::byte>> {
    void operator ()(const std::vector<std::byte>& value,
        std::string& column) const {
      static constexpr auto HEX_DIGITS = "0123456789ABCDEF";
      column += "X'";
      for(auto& byte : value) {
        column += HEX_DIGITS[static_cast<unsigned char>(byte >> 4) & 0xF];
        column += HEX_DIGITS[static_cast<unsigned char>(byte) & 0xF];
      }
      column += '\'';
    }
  };

  template<>
  struct FromSql<std::vector<std::byte>> {
    auto operator ()(const RawColumn& column) const {
      auto value = std::vector<std::byte>(column.m_size);
      std::memcpy(value.data(), column.m_data, column.m_size);
      return value;
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
    std::optional<T> operator ()(const RawColumn& column) const {
      if(column.m_data == nullptr) {
        return std::nullopt;
      }
      return from_sql<T>(column);
    }
  };
}

#endif
