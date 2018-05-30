#ifndef VIPER_INFIX_OPERATOR_HPP
#define VIPER_INFIX_OPERATOR_HPP
#include <stdexcept>
#include "viper/expressions/expression.hpp"

namespace viper {

  //! Implements an SQL expression representing an infix operator.
  class infix_operator final : public virtual_expression {
    public:

      //! Enumerates the types of infix operators.
      enum class type {

        //! left + right
        ADD,

        //! left - right
        SUBTRACT,

        //! left * right
        MULTIPLY,

        //! left / right
        DIVIDE,

        //! left < right
        LESS_THAN,

        //! left <= right
        LESS_THAN_OR_EQUAL,

        //! left = right
        EQUAL,

        //! left <> right
        NOT_EQUAL,

        //! left >= right
        GREATER_THAN_OR_EQUAL,

        //! left > right
        GREATER_THAN,

        //! left AND right
        AND,

        //! left OR right
        OR
      };

      //! Constructs an infix operator.
      /*!
        \param t The operation to represent.
        \param left The left hand side.
        \param right The right hand side.
      */
      infix_operator(type t, expression left, expression right);

      void append_query(std::string& query) const override;

    private:
      type m_type;
      expression m_left;
      expression m_right;
  };

  //! Returns the symbol used to represent an infix operator.
  inline const std::string& get_symbol(infix_operator::type t) {
    if(t == infix_operator::type::ADD) {
      static const std::string SYMBOL = "+";
      return SYMBOL;
    } else if(t == infix_operator::type::SUBTRACT) {
      static const std::string SYMBOL = "-";
      return SYMBOL;
    } else if(t == infix_operator::type::MULTIPLY) {
      static const std::string SYMBOL = "*";
      return SYMBOL;
    } else if(t == infix_operator::type::DIVIDE) {
      static const std::string SYMBOL = "/";
      return SYMBOL;
    } else if(t == infix_operator::type::LESS_THAN) {
      static const std::string SYMBOL = "<";
      return SYMBOL;
    } else if(t == infix_operator::type::LESS_THAN_OR_EQUAL) {
      static const std::string SYMBOL = "<=";
      return SYMBOL;
    } else if(t == infix_operator::type::EQUAL) {
      static const std::string SYMBOL = "=";
      return SYMBOL;
    } else if(t == infix_operator::type::NOT_EQUAL) {
      static const std::string SYMBOL = "<>";
      return SYMBOL;
    } else if(t == infix_operator::type::GREATER_THAN_OR_EQUAL) {
      static const std::string SYMBOL = ">=";
      return SYMBOL;
    } else if(t == infix_operator::type::GREATER_THAN) {
      static const std::string SYMBOL = ">";
      return SYMBOL;
    } else if(t == infix_operator::type::AND) {
      static const std::string SYMBOL = "AND";
      return SYMBOL;
    } else if(t == infix_operator::type::OR) {
      static const std::string SYMBOL = "OR";
      return SYMBOL;
    } else {
      throw std::runtime_error("Invalid operator.");
    }
  }

  //! Returns an expression representing a greater than operation.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator >(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::GREATER_THAN, std::move(left), std::move(right)));
  }

  //! Returns an expression representing a greater than or equal operation.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator >=(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::GREATER_THAN_OR_EQUAL, std::move(left),
      std::move(right)));
  }

  //! Returns an expression representing an equal operation.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator ==(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::EQUAL, std::move(left), std::move(right)));
  }

  //! Returns an expression representing a less than or equal operation.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator <=(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::LESS_THAN_OR_EQUAL, std::move(left),
      std::move(right)));
  }

  //! Returns an expression representing a less than operation.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator <(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::LESS_THAN, std::move(left), std::move(right)));
  }

  inline infix_operator::infix_operator(type t, expression left,
      expression right)
      : m_type(t),
        m_left(std::move(left)),
        m_right(std::move(right)) {}

  inline void infix_operator::append_query(std::string& query) const {
    query += '(';
    m_left.append_query(query);
    query += " " + get_symbol(m_type) + " ";
    m_right.append_query(query);
    query += ')';
  }
}

#endif
