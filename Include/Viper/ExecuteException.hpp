#ifndef VIPER_EXECUTE_EXCEPTION_HPP
#define VIPER_EXECUTE_EXCEPTION_HPP
#include <source_location>
#include <stdexcept>
#include <string>

namespace Viper {

  //! Indicates a failure to execute an SQL statement.
  class ExecuteException : public std::runtime_error {
    public:

      //! Constructs an ExecuteException.
      /*!
        \param message The message describing the failure.
        \param location The location where the failure was detected.
      */
      explicit ExecuteException(const std::string& message,
        std::source_location location = std::source_location::current());

      //! Returns the location where the failure was detected.
      const std::source_location& get_location() const;

    private:
      std::source_location m_location;
  };

  inline ExecuteException::ExecuteException(const std::string& message,
      std::source_location location)
      : std::runtime_error(message),
        m_location(location) {}

  inline const std::source_location& ExecuteException::get_location() const {
    return m_location;
  }
}

#endif
