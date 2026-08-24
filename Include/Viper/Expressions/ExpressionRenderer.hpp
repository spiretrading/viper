#ifndef VIPER_EXPRESSION_RENDERER_HPP
#define VIPER_EXPRESSION_RENDERER_HPP
#include <string>
#include "Viper/Expressions/Expression.hpp"
#include "Viper/Expressions/ExpressionVisitor.hpp"
#include "Viper/Expressions/FunctionExpression.hpp"
#include "Viper/Expressions/InExpression.hpp"
#include "Viper/Expressions/InfixOperator.hpp"
#include "Viper/Expressions/LiteralExpression.hpp"
#include "Viper/Expressions/MembershipOperator.hpp"
#include "Viper/Expressions/NotExpression.hpp"
#include "Viper/Expressions/SymbolExpression.hpp"

namespace Viper {

  //! Renders an expression into an SQL query string, deferring the spelling
  //! of a function to the dialect being rendered for.
  class ExpressionRenderer : public ExpressionVisitor {
    public:

      //! Constructs an expression renderer.
      /*!
        \param query The string to render into.
      */
      explicit ExpressionRenderer(std::string& query);

      void visit(const FunctionExpression& expression) override;

      void visit(const InExpression& expression) override;

      void visit(const InfixOperator& expression) override;

      void visit(const LiteralExpression& expression) override;

      void visit(const MembershipOperator& expression) override;

      void visit(const NotExpression& expression) override;

      void visit(const SymbolExpression& expression) override;

    protected:

      //! Returns the name a function is spelled with.
      /*!
        \param name The canonical name of the function being called.
        \return The name to render the function with.
      */
      virtual std::string get_name(const std::string& name) const = 0;

      //! Renders a sub-expression.
      /*!
        \param expression The sub-expression to render.
      */
      void render(const Expression& expression);

    private:
      std::string* m_query;
  };

  inline ExpressionRenderer::ExpressionRenderer(std::string& query)
    : m_query(&query) {}

  inline void ExpressionRenderer::render(const Expression& expression) {
    expression.apply(*this);
  }

  inline void ExpressionRenderer::visit(const FunctionExpression& expression) {
    *m_query += get_name(expression.get_name());
    *m_query += '(';
    auto is_first = true;
    for(auto& argument : expression.get_arguments()) {
      if(is_first) {
        is_first = false;
      } else {
        *m_query += ", ";
      }
      render(argument);
    }
    *m_query += ')';
  }

  inline void ExpressionRenderer::visit(const InExpression& expression) {
    if(expression.get_set().empty()) {
      render(literal(false));
      return;
    }
    *m_query += '(';
    render(expression.get_value());
    *m_query += " IN (";
    auto is_first = true;
    for(auto& value : expression.get_set()) {
      if(is_first) {
        is_first = false;
      } else {
        *m_query += ", ";
      }
      render(value);
    }
    *m_query += "))";
  }

  inline void ExpressionRenderer::visit(const InfixOperator& expression) {
    *m_query += '(';
    render(expression.get_left());
    *m_query += " " + get_symbol(expression.get_type()) + " ";
    render(expression.get_right());
    *m_query += ')';
  }

  inline void ExpressionRenderer::visit(const LiteralExpression& expression) {
    *m_query += expression.get_value();
  }

  inline void ExpressionRenderer::visit(
      const MembershipOperator& expression) {
    auto size = m_query->size();
    render(expression.get_term());
    if(m_query->size() != size) {
      *m_query += '.';
    }
    *m_query += expression.get_member();
  }

  inline void ExpressionRenderer::visit(const NotExpression& expression) {
    *m_query += "(NOT ";
    render(expression.get_operand());
    *m_query += ")";
  }

  inline void ExpressionRenderer::visit(const SymbolExpression& expression) {
    *m_query += expression.get_symbol();
  }
}

#endif
