#ifndef VIPER_INFIX_OPERATOR_HPP
#define VIPER_INFIX_OPERATOR_HPP
#include <stdexcept>
#include "viper/expressions/expression.hpp"
#include "viper/expressions/literal_expression.hpp"

namespace Viper {

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

  //! Returns an expression representing addition.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator +(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::ADD, std::move(left), std::move(right)));
  }

  //! Returns an expression representing addition.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator +(expression left, const T& right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::ADD, std::move(left), literal(right)));
  }

  //! Returns an expression representing addition.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator +(const T& left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::ADD, literal(left), std::move(right)));
  }

  //! Returns an expression representing subtraction.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator -(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::SUBTRACT, std::move(left), std::move(right)));
  }

  //! Returns an expression representing subtraction.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator -(expression left, const T& right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::SUBTRACT, std::move(left), literal(right)));
  }

  //! Returns an expression representing subtraction.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator -(const T& left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::SUBTRACT, literal(left), std::move(right)));
  }

  //! Returns an expression representing multiplication.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator *(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::MULTIPLY, std::move(left), std::move(right)));
  }

  //! Returns an expression representing multiplication.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator *(expression left, const T& right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::MULTIPLY, std::move(left), literal(right)));
  }

  //! Returns an expression representing multiplication.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator *(const T& left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::MULTIPLY, literal(left), std::move(right)));
  }

  //! Returns an expression representing division.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator /(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::DIVIDE, std::move(left), std::move(right)));
  }

  //! Returns an expression representing division.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator /(expression left, const T& right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::DIVIDE, std::move(left), literal(right)));
  }

  //! Returns an expression representing division.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator /(const T& left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::DIVIDE, literal(left), std::move(right)));
  }

  //! Returns an expression representing less than.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator <(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::LESS_THAN, std::move(left), std::move(right)));
  }

  //! Returns an expression representing less than.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator <(expression left, const T& right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::LESS_THAN, std::move(left), literal(right)));
  }

  //! Returns an expression representing less than.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator <(const T& left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::LESS_THAN, literal(left), std::move(right)));
  }

  //! Returns an expression representing less than or equal.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator <=(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::LESS_THAN_OR_EQUAL, std::move(left),
      std::move(right)));
  }

  //! Returns an expression representing less than or equal.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator <=(expression left, const T& right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::LESS_THAN_OR_EQUAL, std::move(left),
      literal(right)));
  }

  //! Returns an expression representing less than or equal.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator <=(const T& left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::LESS_THAN_OR_EQUAL, literal(left),
      std::move(right)));
  }

  //! Returns an expression representing equality.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator ==(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::EQUAL, std::move(left), std::move(right)));
  }

  //! Returns an expression representing equality.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator ==(expression left, const T& right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::EQUAL, std::move(left), literal(right)));
  }

  //! Returns an expression representing equality.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator ==(const T& left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::EQUAL, literal(left), std::move(right)));
  }

  //! Returns an expression representing inequality.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator !=(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::NOT_EQUAL, std::move(left), std::move(right)));
  }

  //! Returns an expression representing inequality.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator !=(expression left, const T& right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::NOT_EQUAL, std::move(left), literal(right)));
  }

  //! Returns an expression representing inequality.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator !=(const T& left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::NOT_EQUAL, literal(left), std::move(right)));
  }

  //! Returns an expression representing greater than or equal.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator >=(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::GREATER_THAN_OR_EQUAL, std::move(left),
      std::move(right)));
  }

  //! Returns an expression representing greater than or equal.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator >=(expression left, const T& right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::GREATER_THAN_OR_EQUAL, std::move(left),
      literal(right)));
  }

  //! Returns an expression representing greater than or equal.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator >=(const T& left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::GREATER_THAN_OR_EQUAL, literal(left),
      std::move(right)));
  }

  //! Returns an expression representing greater than.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator >(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::GREATER_THAN, std::move(left), std::move(right)));
  }

  //! Returns an expression representing greater than.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator >(expression left, const T& right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::GREATER_THAN, std::move(left), literal(right)));
  }

  //! Returns an expression representing greater than.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator >(const T& left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::GREATER_THAN, literal(left), std::move(right)));
  }

  //! Returns an expression representing logical and.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator &&(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::AND, std::move(left), std::move(right)));
  }

  //! Returns an expression representing logical and.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator &&(expression left, const T& right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::AND, std::move(left), literal(right)));
  }

  //! Returns an expression representing logical and.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator &&(const T& left, expression right) {
    return expression(std::make_shared<infix_operator>(
      infix_operator::type::AND, literal(left), std::move(right)));
  }

  //! Returns an expression representing logical or.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline expression operator ||(expression left, expression right) {
    return expression(std::make_shared<infix_operator>(infix_operator::type::OR,
      std::move(left), std::move(right)));
  }

  //! Returns an expression representing logical or.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator ||(expression left, const T& right) {
    return expression(std::make_shared<infix_operator>(infix_operator::type::OR,
      std::move(left), literal(right)));
  }

  //! Returns an expression representing logical or.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  expression operator ||(const T& left, expression right) {
    return expression(std::make_shared<infix_operator>(infix_operator::type::OR,
      literal(left), std::move(right)));
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
