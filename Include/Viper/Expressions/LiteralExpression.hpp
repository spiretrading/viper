#ifndef VIPER_LITERAL_EXPRESSION_HPP
#define VIPER_LITERAL_EXPRESSION_HPP
#include <string>
#include "Viper/Conversions.hpp"
#include "Viper/Expressions/Expression.hpp"

namespace Viper {

  //! Implements an SQL expression representing a literal value.
  class LiteralExpression final : public VirtualExpression {
    public:

      //! Constructs a literal expression.
      /*!
        \param value The value to represent.
      */
      LiteralExpression(std::string value);

      //! Returns the value.
      const std::string& get_value() const;

      void apply(ExpressionVisitor& visitor) const override;

    private:
      std::string m_value;
  };

  //! Makes a literal expression from a raw value.
  template<typename T>
  Expression literal(const T& value) {
    auto v = std::string();
    to_sql(value, v);
    return Expression(std::make_shared<LiteralExpression>(std::move(v)));
  }

  inline LiteralExpression::LiteralExpression(std::string value)
      : m_value(std::move(value)) {}

  inline const std::string& LiteralExpression::get_value() const {
    return m_value;
  }

  inline void LiteralExpression::apply(ExpressionVisitor& visitor) const {
    visitor.visit(*this);
  }

  inline void ExpressionVisitor::visit(const LiteralExpression& expression) {
    visit(static_cast<const VirtualExpression&>(expression));
  }
}

#endif
