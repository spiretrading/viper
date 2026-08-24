#ifndef VIPER_MEMBERSHIP_OPERATOR_HPP
#define VIPER_MEMBERSHIP_OPERATOR_HPP
#include <string>
#include "Viper/Expressions/Expression.hpp"

namespace Viper {

  //! Implements the membership (dot) operator.
  class MembershipOperator final : public VirtualExpression {
    public:

      //! Constructs a membership operator.
      /*!
        \param term The term to access.
        \param member The member to access from the term.
      */
      MembershipOperator(Expression term, std::string member);

      //! Returns the term.
      const Expression& get_term() const;

      //! Returns the member.
      const std::string& get_member() const;

      void apply(ExpressionVisitor& visitor) const override;

    private:
      Expression m_term;
      std::string m_member;
  };

  //! Accesses a member of an SQL expression.
  /*!
    \param term The term to access.
    \param member The member to access from the term.
  */
  inline auto access(Expression term, std::string member) {
    return Expression(std::make_shared<MembershipOperator>(
      std::move(term), std::move(member)));
  }

  inline MembershipOperator::MembershipOperator(Expression term,
      std::string member)
      : m_term(std::move(term)),
        m_member(std::move(member)) {}

  inline const Expression& MembershipOperator::get_term() const {
    return m_term;
  }

  inline const std::string& MembershipOperator::get_member() const {
    return m_member;
  }

  inline void MembershipOperator::apply(ExpressionVisitor& visitor) const {
    visitor.visit(*this);
  }

  inline void ExpressionVisitor::visit(const MembershipOperator& expression) {
    visit(static_cast<const VirtualExpression&>(expression));
  }
}

#endif
