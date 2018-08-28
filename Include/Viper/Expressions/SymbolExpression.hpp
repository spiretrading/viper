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

      void append_query(std::string& query) const override;

    private:
      std::string m_symbol;
  };

  //! Makes a symbol.
  inline Expression sym(std::string symbol) {
    return Expression(std::make_shared<SymbolExpression>(std::move(symbol)));
  }

  inline SymbolExpression::SymbolExpression(std::string symbol)
      : m_symbol(std::move(symbol)) {}

  inline void SymbolExpression::append_query(std::string& query) const {
    query += m_symbol;
  }
}

#endif
