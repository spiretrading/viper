#ifndef VIPER_EXPRESSION_HPP
#define VIPER_EXPRESSION_HPP
#include <memory>
#include <string>
#include "viper/expressions/virtual_expression.hpp"

namespace viper {

  //! Represents an SQL expression that can be translated into an SQL query
  //! string.
  class expression {
    public:

      //! Constructs an empty expression.
      expression() = default;

      //! Constructs an expression from a polymorphic value.
      /*!
        \param e The value to encapsulate.
      */
      expression(std::shared_ptr<virtual_expression> e);

      //! Appends this expression to an SQL query string.
      void append_query(std::string& query) const;

    private:
      std::shared_ptr<virtual_expression> m_expression;
  };

  inline expression::expression(std::shared_ptr<virtual_expression> e)
      : m_expression(std::move(e)) {}

  inline void expression::append_query(std::string& query) const {
    if(!m_expression) {
      return;
    }
    m_expression->append_query(query);
  }
}

#endif
