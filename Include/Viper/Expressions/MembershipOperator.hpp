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

      void append_query(std::string& query) const override;

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

  inline void MembershipOperator::append_query(std::string& query) const {
    auto initial_size = query.size();
    m_term.append_query(query);
    if(query.size() != initial_size) {
      query += '.';
    }
    query += m_member;
  }
}

#endif
