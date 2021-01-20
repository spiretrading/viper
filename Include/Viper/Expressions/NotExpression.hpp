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

      void append_query(std::string& query) const override;

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

  inline void NotExpression::append_query(std::string& query) const {
    query += "(NOT ";
    m_operand.append_query(query);
    query += ")";
  }
}

#endif
