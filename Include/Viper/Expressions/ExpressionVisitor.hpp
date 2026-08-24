#ifndef VIPER_EXPRESSION_VISITOR_HPP
#define VIPER_EXPRESSION_VISITOR_HPP

namespace Viper {
  class FunctionExpression;
  class InExpression;
  class InfixOperator;
  class LiteralExpression;
  class MembershipOperator;
  class NotExpression;
  class SymbolExpression;
  class VirtualExpression;

  //! Implements the visitor pattern for SQL expressions.
  class ExpressionVisitor {
    public:
      virtual ~ExpressionVisitor() = default;

      virtual void visit(const FunctionExpression& expression);

      virtual void visit(const InExpression& expression);

      virtual void visit(const InfixOperator& expression);

      virtual void visit(const LiteralExpression& expression);

      virtual void visit(const MembershipOperator& expression);

      virtual void visit(const NotExpression& expression);

      virtual void visit(const SymbolExpression& expression);

      virtual void visit(const VirtualExpression& expression);

    protected:

      //! Constructs an SQL expression visitor.
      ExpressionVisitor() = default;
  };

  inline void ExpressionVisitor::visit(const VirtualExpression& expression) {}
}

#endif
