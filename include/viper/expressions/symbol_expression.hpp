#ifndef VIPER_SYMBOL_EXPRESSION_HPP
#define VIPER_SYMBOL_EXPRESSION_HPP
#include <string>
#include "viper/expressions/expression.hpp"

namespace Viper {

  //! Implements an SQL expression representing a symbol.
  class symbol_expression final : public virtual_expression {
    public:

      //! Constructs a symbol.
      /*!
        \param symbol The symbol to represent.
      */
      symbol_expression(std::string symbol);

      void append_query(std::string& query) const override;

    private:
      std::string m_symbol;
  };

  //! Makes a symbol.
  inline expression sym(std::string symbol) {
    return expression(std::make_shared<symbol_expression>(std::move(symbol)));
  }

  inline symbol_expression::symbol_expression(std::string symbol)
      : m_symbol(std::move(symbol)) {}

  inline void symbol_expression::append_query(std::string& query) const {
    query += m_symbol;
  }
}

#endif
