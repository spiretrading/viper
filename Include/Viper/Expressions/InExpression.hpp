#ifndef VIPER_IN_EXPRESSION_HPP
#define VIPER_IN_EXPRESSION_HPP
#include <utility>
#include <vector>
#include "Viper/Expressions/Expression.hpp"
#include "Viper/Expressions/LiteralExpression.hpp"

namespace Viper {

  //! Implements an SQL expression testing for membership in a set of values.
  class InExpression final : public VirtualExpression {
    public:

      //! Constructs an in expression.
      /*!
        \param value The value to test for membership.
        \param set The set of values to test against.
      */
      InExpression(Expression value, std::vector<Expression> set);

      void append_query(std::string& query) const override;

    private:
      Expression m_value;
      std::vector<Expression> m_set;
  };

  //! Returns an expression testing whether a value belongs to a set.
  /*!
    \param value The value to test for membership.
    \param set The set of values to test against.
  */
  inline Expression in(Expression value, std::vector<Expression> set) {
    return Expression(
      std::make_shared<InExpression>(std::move(value), std::move(set)));
  }

  //! Returns an expression testing whether a value belongs to a set.
  /*!
    \param value The value to test for membership.
    \param begin An iterator to the first value to test against.
    \param end An iterator one past the last value to test against.
  */
  template<typename B, typename E>
  Expression in(Expression value, B begin, E end) {
    auto set = std::vector<Expression>();
    for(auto i = begin; i != end; ++i) {
      set.push_back(literal(*i));
    }
    return in(std::move(value), std::move(set));
  }

  inline InExpression::InExpression(
    Expression value, std::vector<Expression> set)
    : m_value(std::move(value)),
      m_set(std::move(set)) {}

  inline void InExpression::append_query(std::string& query) const {
    if(m_set.empty()) {
      literal(false).append_query(query);
      return;
    }
    query += '(';
    m_value.append_query(query);
    query += " IN (";
    auto is_first = true;
    for(auto& value : m_set) {
      if(is_first) {
        is_first = false;
      } else {
        query += ", ";
      }
      value.append_query(query);
    }
    query += "))";
  }
}

#endif
