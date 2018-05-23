#ifndef VIPER_SQL_DATA_TYPE_HPP
#define VIPER_SQL_DATA_TYPE_HPP
#include <type_traits>

namespace viper {

  //! Lists the supported SQL data types.
  enum class sql_data_type {
    INTEGER
  };

  //! Maps a native C++ type to its SQL data type.
  template<typename T>
  struct to_sql_type {};

  template<typename T>
  constexpr auto to_sql_type_v = to_sql_type<std::decay_t<T>>::value;

  template<>
  struct to_sql_type<int> {
    static constexpr auto value = sql_data_type::INTEGER;
  };
}

#endif
