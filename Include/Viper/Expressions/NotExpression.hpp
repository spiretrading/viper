#ifndef VIPER_NOT_EXPRESSION_HPP
#define VIPER_NOT_EXPRESSION_HPP
#include "Viper/Expressions/Expression.hpp"

namespace Viper {

  //! Implements an SQL expression representing the not operator.
  class NotExpression final : public VirtualExpression {
    public:

      //! Constructs a not expression.
      /*!
        \param operand The operand to negate.
      */
      NotExpression(Expression operand);

      //! Returns the operand.
      const Expression& get_operand() const;

      void apply(ExpressionVisitor& visitor) const override;

    private:
      Expression m_operand;
  };

  //! Returns an expression negating its operand.
  /*!
    \param operand The operand to negate.
  */
  inline Expression operator !(Expression operand) {
    return Expression(std::make_shared<NotExpression>(std::move(operand)));
  }

  inline NotExpression::NotExpression(Expression operand)
    : m_operand(std::move(operand)) {}

  inline const Expression& NotExpression::get_operand() const {
    return m_operand;
  }

  inline void NotExpression::apply(ExpressionVisitor& visitor) const {
    visitor.visit(*this);
  }

  inline void ExpressionVisitor::visit(const NotExpression& expression) {
    visit(static_cast<const VirtualExpression&>(expression));
  }
}

#endif
