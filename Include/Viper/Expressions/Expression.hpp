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

      //! Appends this expression to an SQL query string.
      void append_query(std::string& query) const;

    private:
      std::shared_ptr<VirtualExpression> m_expression;
  };

  inline Expression::Expression(std::shared_ptr<VirtualExpression> e)
      : m_expression(std::move(e)) {}

  inline void Expression::append_query(std::string& query) const {
    if(!m_expression) {
      return;
    }
    m_expression->append_query(query);
  }
}

#endif
