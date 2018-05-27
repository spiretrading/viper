#ifndef VIPER_NATIVE_TO_DATA_TYPE_HPP
#define VIPER_NATIVE_TO_DATA_TYPE_HPP
#include <cstdint>
#include <string>
#include "viper/data_types/float_data_type.hpp"
#include "viper/data_types/integer_data_type.hpp"
#include "viper/data_types/varchar_data_type.hpp"

namespace viper {

  //! Returns the SQL data type that best represents a native C++ type.
  template<typename T>
  auto native_to_data_type_v = integer_data_type(true, 0);

  template<>
  auto native_to_data_type_v<std::int8_t> = integer_data_type(true, 1);

  template<>
  auto native_to_data_type_v<std::uint8_t> = integer_data_type(false, 1);

  template<>
  auto native_to_data_type_v<std::int16_t> = integer_data_type(true, 2);

  template<>
  auto native_to_data_type_v<std::uint16_t> = integer_data_type(false, 2);

  template<>
  auto native_to_data_type_v<std::int32_t> = integer_data_type(true, 4);

  template<>
  auto native_to_data_type_v<std::uint32_t> = integer_data_type(false, 4);

  template<>
  auto native_to_data_type_v<std::int64_t> = integer_data_type(true, 8);

  template<>
  auto native_to_data_type_v<std::uint64_t> = integer_data_type(false, 8);

  template<>
  auto native_to_data_type_v<float> = float_data_type(4);

  template<>
  auto native_to_data_type_v<double> = float_data_type(8);

  template<>
  auto native_to_data_type_v<std::string> = varchar_data_type();
}

#endif
