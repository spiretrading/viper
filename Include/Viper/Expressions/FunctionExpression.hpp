#ifndef VIPER_FUNCTION_EXPRESSION_HPP
#define VIPER_FUNCTION_EXPRESSION_HPP
#include <string>
#include <utility>
#include <vector>
#include "Viper/Expressions/Expression.hpp"

namespace Viper {

  //! The name used for the function returning the greater of its arguments.
  inline const auto GREATEST_NAME = std::string("GREATEST");

  //! The name used for the function returning the lesser of its arguments.
  inline const auto LEAST_NAME = std::string("LEAST");

  //! Implements an SQL expression representing a call to a function.
  class FunctionExpression final : public VirtualExpression {
    public:

      //! Constructs a function expression.
      /*!
        \param name The name of the function to call.
        \param arguments The arguments to pass to the function.
      */
      FunctionExpression(std::string name, std::vector<Expression> arguments);

      //! Returns the name of the function being called.
      const std::string& get_name() const;

      //! Returns the arguments.
      const std::vector<Expression>& get_arguments() const;

      void apply(ExpressionVisitor& visitor) const override;

    private:
      std::string m_name;
      std::vector<Expression> m_arguments;
  };

  //! Returns an expression representing a call to a function.
  /*!
    \param name The name of the function to call.
    \param arguments The arguments to pass to the function.
  */
  inline Expression call(std::string name, std::vector<Expression> arguments) {
    return Expression(std::make_shared<FunctionExpression>(
      std::move(name), std::move(arguments)));
  }

  //! Returns an expression evaluating to the greater of two values.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression greatest(Expression left, Expression right) {
    return call(GREATEST_NAME, {std::move(left), std::move(right)});
  }

  //! Returns an expression evaluating to the lesser of two values.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression least(Expression left, Expression right) {
    return call(LEAST_NAME, {std::move(left), std::move(right)});
  }

  inline FunctionExpression::FunctionExpression(
    std::string name, std::vector<Expression> arguments)
    : m_name(std::move(name)),
      m_arguments(std::move(arguments)) {}

  inline const std::string& FunctionExpression::get_name() const {
    return m_name;
  }

  inline const std::vector<Expression>&
      FunctionExpression::get_arguments() const {
    return m_arguments;
  }

  inline void FunctionExpression::apply(ExpressionVisitor& visitor) const {
    visitor.visit(*this);
  }

  inline void ExpressionVisitor::visit(const FunctionExpression& expression) {
    visit(static_cast<const VirtualExpression&>(expression));
  }
}

#endif
