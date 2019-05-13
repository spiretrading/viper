#ifndef VIPER_UPDATE_STATEMENT_HPP
#define VIPER_UPDATE_STATEMENT_HPP
#include <optional>
#include <string>
#include <tuple>
#include "Viper/Expressions/Expression.hpp"

namespace Viper {

  //! Represents the set clause of an update statement.
  struct SetClause {

    //! The column to set.
    std::string m_column;

    //! The value to assign to the column.
    Expression m_value;

    //! Constructs a set clause.
    /*!
      \param column The column to set.
      \param value The value to assign to the column.
    */
    SetClause(std::string column, Expression value);

    //! Constructs a set clause.
    /*!
      \param column The column to set.
      \param value The value to assign to the column.
    */
    template<typename T>
    SetClause(std::string column, const T& value);
  };

  /*! \brief Represents an SQL UPDATE statement. */
  class UpdateStatement {
    public:

      //! Constructs an update statement.
      /*!
        \param table The name of the table to update.
        \param set The set clause.
      */
      UpdateStatement(std::string table, SetClause set);

      //! Constructs an update statement.
      /*!
        \param table The name of the table to update.
        \param set The set clause.
        \param where The where clause.
      */
      UpdateStatement(std::string table, SetClause set, Expression where);

      //! Returns the name of the table to update.
      const std::string& get_table() const;

      //! Returns the set clause.
      const SetClause& get_set() const;

      //! Returns the where clause.
      const std::optional<Expression>& get_where() const;

    private:
      std::string m_table;
      SetClause m_set;
      std::optional<Expression> m_where;
  };

  //! Builds an update statement.
  /*!
    \param table The name of the table to update.
    \param set The set clause.
  */
  inline auto update(std::string table, SetClause set) {
    return UpdateStatement(std::move(table), std::move(set));
  }

  //! Builds an update statement.
  /*!
    \param table The name of the table to update.
    \param set The set clause.
    \param where The where clause.
  */
  inline auto update(std::string table, SetClause set, Expression where) {
    return UpdateStatement(std::move(table), std::move(set),
      std::move(where));
  }

  inline SetClause::SetClause(std::string column, Expression value)
    : m_column(std::move(column)),
      m_value(std::move(value)) {}

  template<typename T>
  SetClause::SetClause(std::string column, const T& value)
    : SetClause(std::move(column), literal(value)) {}

  inline UpdateStatement::UpdateStatement(std::string table, SetClause set)
      : m_table(std::move(table)),
        m_set(std::move(set)) {}

  inline UpdateStatement::UpdateStatement(std::string table, SetClause set,
      Expression where)
      : m_table(std::move(table)),
        m_set(std::move(set)),
        m_where(std::move(where)) {}

  inline const std::string& UpdateStatement::get_table() const {
    return m_table;
  }

  inline const SetClause& UpdateStatement::get_set() const {
    return m_set;
  }

  inline const std::optional<Expression>& UpdateStatement::get_where() const {
    return m_where;
  }
}

#endif
