#ifndef VIPER_FUNCTION_EXPRESSION_HPP
#define VIPER_FUNCTION_EXPRESSION_HPP
#include <string>
#include <utility>
#include <vector>
#include "Viper/Expressions/Expression.hpp"

namespace Viper {

  //! Implements an SQL expression representing a call to a function.
  class FunctionExpression final : public VirtualExpression {
    public:

      //! Constructs a function expression.
      /*!
        \param name The name of the function to call.
        \param arguments The arguments to pass to the function.
      */
      FunctionExpression(std::string name, std::vector<Expression> arguments);

      void append_query(std::string& query) const override;

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

  inline FunctionExpression::FunctionExpression(
    std::string name, std::vector<Expression> arguments)
    : m_name(std::move(name)),
      m_arguments(std::move(arguments)) {}

  inline void FunctionExpression::append_query(std::string& query) const {
    query += m_name;
    query += '(';
    auto is_first = true;
    for(auto& argument : m_arguments) {
      if(is_first) {
        is_first = false;
      } else {
        query += ", ";
      }
      argument.append_query(query);
    }
    query += ')';
  }
}

#endif
