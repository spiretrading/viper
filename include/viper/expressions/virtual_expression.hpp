#ifndef VIPER_VIRTUAL_EXPRESSION_HPP
#define VIPER_VIRTUAL_EXPRESSION_HPP
#include <string>

namespace viper {

  //! Base class of an SQL expression.
  class virtual_expression {
    public:
      virtual ~virtual_expression() = default;

      //! Appends this expression to an SQL query string.
      virtual void append_query(std::string& query) const = 0;

    protected:

      //! Constructs a virtual expression.
      virtual_expression() = default;

    private:
      virtual_expression(const virtual_expression&) = delete;
      virtual_expression& operator =(const virtual_expression&) = delete;
  };
}

#endif
