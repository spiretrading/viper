#ifndef VIPER_VIRTUAL_EXPRESSION_HPP
#define VIPER_VIRTUAL_EXPRESSION_HPP
#include "Viper/Expressions/ExpressionVisitor.hpp"

namespace Viper {

  //! Base class of an SQL expression.
  class VirtualExpression {
    public:
      virtual ~VirtualExpression() = default;

      //! Applies a visitor to this expression.
      virtual void apply(ExpressionVisitor& visitor) const = 0;

    protected:

      //! Constructs a virtual expression.
      VirtualExpression() = default;

    private:
      VirtualExpression(const VirtualExpression&) = delete;
      VirtualExpression& operator =(const VirtualExpression&) = delete;
  };
}

#endif
