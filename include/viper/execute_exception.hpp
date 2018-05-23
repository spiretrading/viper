#ifndef VIPER_EXECUTE_EXCEPTION_HPP
#define VIPER_EXECUTE_EXCEPTION_HPP
#include <stdexcept>

namespace viper {

  //! Indicates a failure to execute an SQL statement.
  class execute_exception : public std::runtime_error {
    public:
      using runtime_error::runtime_error;
  };
}

#endif
