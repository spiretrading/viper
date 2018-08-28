#ifndef VIPER_EXECUTE_EXCEPTION_HPP
#define VIPER_EXECUTE_EXCEPTION_HPP
#include <stdexcept>

namespace Viper {

  //! Indicates a failure to execute an SQL statement.
  class ExecuteException : public std::runtime_error {
    public:
      using runtime_error::runtime_error;
  };
}

#endif
