#ifndef VIPER_INFIX_OPERATOR_HPP
#define VIPER_INFIX_OPERATOR_HPP
#include <stdexcept>
#include "viper/expressions/expression.hpp"
#include "viper/expressions/literal_expression.hpp"

namespace Viper {

  //! Implements an SQL expression representing an infix operator.
  class InfixOperator final : public VirtualExpression {
    public:

      //! Enumerates the types of infix operators.
      enum class Type {

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
      InfixOperator(Type t, Expression left, Expression right);

      void append_query(std::string& query) const override;

    private:
      Type m_type;
      Expression m_left;
      Expression m_right;
  };

  //! Returns the symbol used to represent an infix operator.
  inline const std::string& get_symbol(InfixOperator::Type t) {
    if(t == InfixOperator::Type::ADD) {
      static const std::string SYMBOL = "+";
      return SYMBOL;
    } else if(t == InfixOperator::Type::SUBTRACT) {
      static const std::string SYMBOL = "-";
      return SYMBOL;
    } else if(t == InfixOperator::Type::MULTIPLY) {
      static const std::string SYMBOL = "*";
      return SYMBOL;
    } else if(t == InfixOperator::Type::DIVIDE) {
      static const std::string SYMBOL = "/";
      return SYMBOL;
    } else if(t == InfixOperator::Type::LESS_THAN) {
      static const std::string SYMBOL = "<";
      return SYMBOL;
    } else if(t == InfixOperator::Type::LESS_THAN_OR_EQUAL) {
      static const std::string SYMBOL = "<=";
      return SYMBOL;
    } else if(t == InfixOperator::Type::EQUAL) {
      static const std::string SYMBOL = "=";
      return SYMBOL;
    } else if(t == InfixOperator::Type::NOT_EQUAL) {
      static const std::string SYMBOL = "<>";
      return SYMBOL;
    } else if(t == InfixOperator::Type::GREATER_THAN_OR_EQUAL) {
      static const std::string SYMBOL = ">=";
      return SYMBOL;
    } else if(t == InfixOperator::Type::GREATER_THAN) {
      static const std::string SYMBOL = ">";
      return SYMBOL;
    } else if(t == InfixOperator::Type::AND) {
      static const std::string SYMBOL = "AND";
      return SYMBOL;
    } else if(t == InfixOperator::Type::OR) {
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
  inline Expression operator +(Expression left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::ADD, std::move(left), std::move(right)));
  }

  //! Returns an expression representing addition.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator +(Expression left, const T& right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::ADD, std::move(left), literal(right)));
  }

  //! Returns an expression representing addition.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator +(const T& left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::ADD, literal(left), std::move(right)));
  }

  //! Returns an expression representing subtraction.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression operator -(Expression left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::SUBTRACT, std::move(left), std::move(right)));
  }

  //! Returns an expression representing subtraction.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator -(Expression left, const T& right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::SUBTRACT, std::move(left), literal(right)));
  }

  //! Returns an expression representing subtraction.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator -(const T& left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::SUBTRACT, literal(left), std::move(right)));
  }

  //! Returns an expression representing multiplication.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression operator *(Expression left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::MULTIPLY, std::move(left), std::move(right)));
  }

  //! Returns an expression representing multiplication.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator *(Expression left, const T& right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::MULTIPLY, std::move(left), literal(right)));
  }

  //! Returns an expression representing multiplication.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator *(const T& left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::MULTIPLY, literal(left), std::move(right)));
  }

  //! Returns an expression representing division.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression operator /(Expression left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::DIVIDE, std::move(left), std::move(right)));
  }

  //! Returns an expression representing division.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator /(Expression left, const T& right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::DIVIDE, std::move(left), literal(right)));
  }

  //! Returns an expression representing division.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator /(const T& left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::DIVIDE, literal(left), std::move(right)));
  }

  //! Returns an expression representing less than.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression operator <(Expression left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::LESS_THAN, std::move(left), std::move(right)));
  }

  //! Returns an expression representing less than.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator <(Expression left, const T& right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::LESS_THAN, std::move(left), literal(right)));
  }

  //! Returns an expression representing less than.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator <(const T& left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::LESS_THAN, literal(left), std::move(right)));
  }

  //! Returns an expression representing less than or equal.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression operator <=(Expression left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::LESS_THAN_OR_EQUAL, std::move(left),
      std::move(right)));
  }

  //! Returns an expression representing less than or equal.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator <=(Expression left, const T& right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::LESS_THAN_OR_EQUAL, std::move(left),
      literal(right)));
  }

  //! Returns an expression representing less than or equal.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator <=(const T& left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::LESS_THAN_OR_EQUAL, literal(left),
      std::move(right)));
  }

  //! Returns an expression representing equality.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression operator ==(Expression left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::EQUAL, std::move(left), std::move(right)));
  }

  //! Returns an expression representing equality.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator ==(Expression left, const T& right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::EQUAL, std::move(left), literal(right)));
  }

  //! Returns an expression representing equality.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator ==(const T& left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::EQUAL, literal(left), std::move(right)));
  }

  //! Returns an expression representing inequality.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression operator !=(Expression left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::NOT_EQUAL, std::move(left), std::move(right)));
  }

  //! Returns an expression representing inequality.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator !=(Expression left, const T& right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::NOT_EQUAL, std::move(left), literal(right)));
  }

  //! Returns an expression representing inequality.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator !=(const T& left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::NOT_EQUAL, literal(left), std::move(right)));
  }

  //! Returns an expression representing greater than or equal.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression operator >=(Expression left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::GREATER_THAN_OR_EQUAL, std::move(left),
      std::move(right)));
  }

  //! Returns an expression representing greater than or equal.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator >=(Expression left, const T& right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::GREATER_THAN_OR_EQUAL, std::move(left),
      literal(right)));
  }

  //! Returns an expression representing greater than or equal.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator >=(const T& left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::GREATER_THAN_OR_EQUAL, literal(left),
      std::move(right)));
  }

  //! Returns an expression representing greater than.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression operator >(Expression left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::GREATER_THAN, std::move(left), std::move(right)));
  }

  //! Returns an expression representing greater than.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator >(Expression left, const T& right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::GREATER_THAN, std::move(left), literal(right)));
  }

  //! Returns an expression representing greater than.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator >(const T& left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::GREATER_THAN, literal(left), std::move(right)));
  }

  //! Returns an expression representing logical and.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression operator &&(Expression left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::AND, std::move(left), std::move(right)));
  }

  //! Returns an expression representing logical and.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator &&(Expression left, const T& right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::AND, std::move(left), literal(right)));
  }

  //! Returns an expression representing logical and.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator &&(const T& left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(
      InfixOperator::Type::AND, literal(left), std::move(right)));
  }

  //! Returns an expression representing logical or.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression operator ||(Expression left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(InfixOperator::Type::OR,
      std::move(left), std::move(right)));
  }

  //! Returns an expression representing logical or.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator ||(Expression left, const T& right) {
    return Expression(std::make_shared<InfixOperator>(InfixOperator::Type::OR,
      std::move(left), literal(right)));
  }

  //! Returns an expression representing logical or.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  template<typename T>
  Expression operator ||(const T& left, Expression right) {
    return Expression(std::make_shared<InfixOperator>(InfixOperator::Type::OR,
      literal(left), std::move(right)));
  }

  inline InfixOperator::InfixOperator(Type t, Expression left,
      Expression right)
      : m_type(t),
        m_left(std::move(left)),
        m_right(std::move(right)) {}

  inline void InfixOperator::append_query(std::string& query) const {
    query += '(';
    m_left.append_query(query);
    query += " " + get_symbol(m_type) + " ";
    m_right.append_query(query);
    query += ')';
  }
}

#endif
