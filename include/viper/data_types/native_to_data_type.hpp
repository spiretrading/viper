#ifndef VIPER_NATIVE_TO_DATA_TYPE_HPP
#define VIPER_NATIVE_TO_DATA_TYPE_HPP
#include "viper/data_types/integer_data_type.hpp"

namespace viper {

  //! Returns the SQL data type that best represents a native C++ type.
  template<typename T>
  auto native_to_data_type_v = integer_data_type(true, 0);

  template<>
  auto native_to_data_type_v<std::int32_t> = integer_data_type(true, 4);
}

#endif
