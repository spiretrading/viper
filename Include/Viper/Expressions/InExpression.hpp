#ifndef VIPER_IN_EXPRESSION_HPP
#define VIPER_IN_EXPRESSION_HPP
#include <utility>
#include <vector>
#include "Viper/Expressions/Expression.hpp"
#include "Viper/Expressions/LiteralExpression.hpp"

namespace Viper {

  //! Implements an SQL expression testing for membership in a set of values.
  class InExpression final : public VirtualExpression {
    public:

      //! Constructs an in expression.
      /*!
        \param value The value to test for membership.
        \param set The set of values to test against.
      */
      InExpression(Expression value, std::vector<Expression> set);

      //! Returns the value tested for membership.
      const Expression& get_value() const;

      //! Returns the set.
      const std::vector<Expression>& get_set() const;

      void apply(ExpressionVisitor& visitor) const override;

    private:
      Expression m_value;
      std::vector<Expression> m_set;
  };

  //! Returns an expression testing whether a value belongs to a set.
  /*!
    \param value The value to test for membership.
    \param set The set of values to test against.
  */
  inline Expression in(Expression value, std::vector<Expression> set) {
    return Expression(
      std::make_shared<InExpression>(std::move(value), std::move(set)));
  }

  //! Returns an expression testing whether a value belongs to a set.
  /*!
    \param value The value to test for membership.
    \param begin An iterator to the first value to test against.
    \param end An iterator one past the last value to test against.
  */
  template<typename B, typename E>
  Expression in(Expression value, B begin, E end) {
    auto set = std::vector<Expression>();
    for(auto i = begin; i != end; ++i) {
      set.push_back(literal(*i));
    }
    return in(std::move(value), std::move(set));
  }

  inline InExpression::InExpression(
    Expression value, std::vector<Expression> set)
    : m_value(std::move(value)),
      m_set(std::move(set)) {}

  inline const Expression& InExpression::get_value() const {
    return m_value;
  }

  inline const std::vector<Expression>& InExpression::get_set() const {
    return m_set;
  }

  inline void InExpression::apply(ExpressionVisitor& visitor) const {
    visitor.visit(*this);
  }

  inline void ExpressionVisitor::visit(const InExpression& expression) {
    visit(static_cast<const VirtualExpression&>(expression));
  }
}

#endif
