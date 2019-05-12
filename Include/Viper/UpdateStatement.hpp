#ifndef VIPER_UPDATE_STATEMENT_HPP
#define VIPER_UPDATE_STATEMENT_HPP
#include <string>
#include <tuple>
#include "Viper/Expressions/Expression.hpp"

namespace Viper {

  /*! \brief Represents an SQL UPDATE statement. */
  class UpdateStatement {
    public:

      //! Type used to represent the set clause.
      using SetClause = std::tuple<std::string_view, Expression>;

      //! Constructs an update statement.
      /*!
        \param table The name of the table to update.
        \param set The set clause.
      */
      UpdateStatement(std::string table, SetClause set);

      //! Returns the name of the table to update.
      const std::string& get_table() const;

      //! Returns the set clause.
      const SetClause& get_set() const;

    private:
      std::string m_table;
      SetClause m_set;
  };

  //! Builds an update statement.
  /*!
    \param table The name of the table to update.
    \param set The set clause.
  */
  auto update(std::string table, UpdateStatement::SetClause set) {
    return UpdateStatement(std::move(table), std::move(set));
  }

  inline UpdateStatement::UpdateStatement(std::string table, SetClause set)
      : m_table(std::move(table)),
        m_set(std::move(set)) {}

  inline const std::string& UpdateStatement::get_table() const {
    return m_table;
  }

  inline const UpdateStatement::SetClause& UpdateStatement::get_set() const {
    return m_set;
  }
}

#endif
