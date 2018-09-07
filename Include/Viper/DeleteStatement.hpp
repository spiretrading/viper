#ifndef VIPER_DELETE_STATEMENT_HPP
#define VIPER_DELETE_STATEMENT_HPP
#include <optional>
#include <string>
#include "Viper/Expressions/Expression.hpp"
#include "Viper/Row.hpp"

namespace Viper {

  /** SQL statement used to delete rows from a table. */
  class DeleteStatement {
    public:

      //! Constructs a delete statement.
      /*!
        \param table The table to delete from.
      */
      DeleteStatement(std::string table);

      //! Constructs a delete statement.
      /*!
        \param table The table to delete from.
        \param where The where clause.
      */
      DeleteStatement(std::string table, Expression where);

      //! Returns the table to delete from.
      const std::string& get_table() const;

      //! Returns the where clause.
      const std::optional<Expression>& get_where() const;

    private:
      std::string m_table;
      std::optional<Expression> m_where;
  };

  //! Builds a delete all statement.
  /*!
    \param table The table to delete from.
  */
  inline auto truncate(std::string table) {
    return DeleteStatement(std::move(table));
  }

  inline DeleteStatement::DeleteStatement(std::string table)
      : m_table(std::move(table)) {}

  inline DeleteStatement::DeleteStatement(std::string table,
      Expression where)
      : m_table(std::move(table)),
        m_where(std::move(where)) {}

  inline const std::string& DeleteStatement::get_table() const {
    return m_table;
  }

  inline const std::optional<Expression>& DeleteStatement::get_where() const {
    return m_where;
  }
}

#endif
