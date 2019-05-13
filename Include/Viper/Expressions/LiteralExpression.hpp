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

      void append_query(std::string& query) const override;

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

  inline void LiteralExpression::append_query(std::string& query) const {
    query += m_value;
  }
}

#endif
