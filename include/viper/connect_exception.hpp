#ifndef VIPER_CONNECT_EXCEPTION_HPP
#define VIPER_CONNECT_EXCEPTION_HPP
#include <stdexcept>

namespace Viper {

  //! Indicates a failure to connect to an SQL database.
  class connect_exception : public std::runtime_error {
    public:
      using runtime_error::runtime_error;
  };
}

#endif
