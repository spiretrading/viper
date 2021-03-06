#ifndef VIPER_NATIVE_TO_DATA_TYPE_HPP
#define VIPER_NATIVE_TO_DATA_TYPE_HPP
#include <cstdint>
#include <optional>
#include <string>
#include <type_traits>
#include "Viper/DataTypes/BlobDataType.hpp"
#include "Viper/DataTypes/DateTimeDataType.hpp"
#include "Viper/DataTypes/FloatDataType.hpp"
#include "Viper/DataTypes/IntegerDataType.hpp"
#include "Viper/DataTypes/VarCharDataType.hpp"

namespace Viper {

  //! Returns the SQL data type that best represents a native C++ type.
  template<typename T, typename = void>
  inline const auto native_to_data_type_v =
    std::enable_if_t<!std::is_same_v<T, T>>();

  template<typename T>
  inline const auto native_to_data_type_v<std::optional<T>> =
    native_to_data_type_v<T>;

  template<>
  inline const auto native_to_data_type_v<std::vector<std::byte>> =
    BlobDataType();

  template<>
  inline const auto native_to_data_type_v<DateTime> = DateTimeDataType();

  template<>
  inline const auto native_to_data_type_v<bool> = IntegerDataType(false, 1);

  template<>
  inline const auto native_to_data_type_v<char> = VarCharDataType(1);

  template<>
  inline const auto native_to_data_type_v<std::int8_t> =
    IntegerDataType(true, 1);

  template<>
  inline const auto native_to_data_type_v<std::uint8_t> =
    IntegerDataType(false, 1);

  template<>
  inline const auto native_to_data_type_v<std::int16_t> =
    IntegerDataType(true, 2);

  template<>
  inline const auto native_to_data_type_v<std::uint16_t> =
    IntegerDataType(false, 2);

  template<>
  inline const auto native_to_data_type_v<std::int32_t> =
    IntegerDataType(true, 4);

  template<>
  inline const auto native_to_data_type_v<std::uint32_t> =
    IntegerDataType(false, 4);

  template<>
  inline const auto native_to_data_type_v<std::int64_t> =
    IntegerDataType(true, 8);

  template<>
  inline const auto native_to_data_type_v<std::uint64_t> =
    IntegerDataType(false, 8);

  template<>
  inline const auto native_to_data_type_v<float> = FloatDataType(4);

  template<>
  inline const auto native_to_data_type_v<double> = FloatDataType(8);

  template<>
  inline const auto native_to_data_type_v<std::string> = VarCharDataType();

  template<typename T>
  inline const auto native_to_data_type_v<
    T, std::enable_if_t<std::is_enum_v<T>>> = IntegerDataType(true, 4);
}

#endif
