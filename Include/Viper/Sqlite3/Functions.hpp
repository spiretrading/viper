#ifndef VIPER_SQLITE3_FUNCTIONS_HPP
#define VIPER_SQLITE3_FUNCTIONS_HPP
#include <utility>
#include "Viper/Expressions/FunctionExpression.hpp"

namespace Viper::Sqlite3 {

  //! Returns an expression evaluating to the greater of two values.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression greatest(Expression left, Expression right) {
    return call("MAX", {std::move(left), std::move(right)});
  }

  //! Returns an expression evaluating to the lesser of two values.
  /*!
    \param left The left hand side.
    \param right The right hand side.
  */
  inline Expression least(Expression left, Expression right) {
    return call("MIN", {std::move(left), std::move(right)});
  }
}

#endif
