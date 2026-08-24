#ifndef VIPER_SYMBOL_EXPRESSION_HPP
#define VIPER_SYMBOL_EXPRESSION_HPP
#include <string>
#include "Viper/Expressions/Expression.hpp"

namespace Viper {

  //! Implements an SQL expression representing a symbol.
  class SymbolExpression final : public VirtualExpression {
    public:

      //! Constructs a symbol.
      /*!
        \param symbol The symbol to represent.
      */
      SymbolExpression(std::string symbol);

      //! Returns the symbol.
      const std::string& get_symbol() const;

      void apply(ExpressionVisitor& visitor) const override;

    private:
      std::string m_symbol;
  };

  //! Makes a symbol.
  inline Expression sym(std::string symbol) {
    return Expression(std::make_shared<SymbolExpression>(std::move(symbol)));
  }

  inline SymbolExpression::SymbolExpression(std::string symbol)
      : m_symbol(std::move(symbol)) {}

  inline const std::string& SymbolExpression::get_symbol() const {
    return m_symbol;
  }

  inline void SymbolExpression::apply(ExpressionVisitor& visitor) const {
    visitor.visit(*this);
  }

  inline void ExpressionVisitor::visit(const SymbolExpression& expression) {
    visit(static_cast<const VirtualExpression&>(expression));
  }
}

#endif
