#ifndef VIPER_EXPRESSION_HPP
#define VIPER_EXPRESSION_HPP
#include <memory>
#include <string>
#include "Viper/Expressions/VirtualExpression.hpp"

namespace Viper {

  //! Represents an SQL expression that can be translated into an SQL query
  //! string.
  class Expression {
    public:

      //! Constructs an empty expression.
      Expression() = default;

      //! Constructs an expression from a polymorphic value.
      /*!
        \param e The value to encapsulate.
      */
      Expression(std::shared_ptr<VirtualExpression> e);

      //! Applies a visitor to this expression.
      void apply(ExpressionVisitor& visitor) const;

    private:
      std::shared_ptr<VirtualExpression> m_expression;
  };

  inline Expression::Expression(std::shared_ptr<VirtualExpression> e)
      : m_expression(std::move(e)) {}

  inline void Expression::apply(ExpressionVisitor& visitor) const {
    if(!m_expression) {
      return;
    }
    m_expression->apply(visitor);
  }
}

#endif
