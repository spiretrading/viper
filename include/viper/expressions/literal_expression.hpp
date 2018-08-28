#ifndef VIPER_LITERAL_EXPRESSION_HPP
#define VIPER_LITERAL_EXPRESSION_HPP
#include <string>
#include "viper/expressions/expression.hpp"

namespace Viper {

  //! Implements an SQL expression representing a literal value.
  class literal_expression final : public virtual_expression {
    public:

      //! Constructs a literal expression.
      /*!
        \param value The value to represent.
      */
      literal_expression(std::string value);

      void append_query(std::string& query) const override;

    private:
      std::string m_value;
  };

  //! Makes a literal expression from a raw value.
  template<typename T>
  expression literal(const T& value) {
    std::string v;
    convert_to_sql(value, v);
    return expression(std::make_shared<literal_expression>(std::move(v)));
  }

  inline literal_expression::literal_expression(std::string value)
      : m_value(std::move(value)) {}

  inline void literal_expression::append_query(std::string& query) const {
    query += m_value;
  }
}

#endif
