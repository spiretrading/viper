#ifndef VIPER_NATIVE_TO_DATA_TYPE_HPP
#define VIPER_NATIVE_TO_DATA_TYPE_HPP
#include <cstdint>
#include <string>
#include "Viper/DataTypes/FloatDataType.hpp"
#include "Viper/DataTypes/IntegerDataType.hpp"
#include "Viper/DataTypes/VarCharDataType.hpp"

namespace Viper {

  //! Returns the SQL data type that best represents a native C++ type.
  template<typename T>
  auto native_to_data_type_v = IntegerDataType(true, 0);

  template<>
  auto native_to_data_type_v<std::int8_t> = IntegerDataType(true, 1);

  template<>
  auto native_to_data_type_v<std::uint8_t> = IntegerDataType(false, 1);

  template<>
  auto native_to_data_type_v<std::int16_t> = IntegerDataType(true, 2);

  template<>
  auto native_to_data_type_v<std::uint16_t> = IntegerDataType(false, 2);

  template<>
  auto native_to_data_type_v<std::int32_t> = IntegerDataType(true, 4);

  template<>
  auto native_to_data_type_v<std::uint32_t> = IntegerDataType(false, 4);

  template<>
  auto native_to_data_type_v<std::int64_t> = IntegerDataType(true, 8);

  template<>
  auto native_to_data_type_v<std::uint64_t> = IntegerDataType(false, 8);

  template<>
  auto native_to_data_type_v<float> = FloatDataType(4);

  template<>
  auto native_to_data_type_v<double> = FloatDataType(8);

  template<>
  auto native_to_data_type_v<std::string> = VarCharDataType();
}

#endif
