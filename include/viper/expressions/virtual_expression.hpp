#ifndef VIPER_VIRTUAL_EXPRESSION_HPP
#define VIPER_VIRTUAL_EXPRESSION_HPP
#include <string>

namespace Viper {

  //! Base class of an SQL expression.
  class VirtualExpression {
    public:
      virtual ~VirtualExpression() = default;

      //! Appends this expression to an SQL query string.
      virtual void append_query(std::string& query) const = 0;

    protected:

      //! Constructs a virtual expression.
      VirtualExpression() = default;

    private:
      VirtualExpression(const VirtualExpression&) = delete;
      VirtualExpression& operator =(const VirtualExpression&) = delete;
  };
}

#endif
