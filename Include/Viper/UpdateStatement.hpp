#ifndef VIPER_UPDATE_STATEMENT_HPP
#define VIPER_UPDATE_STATEMENT_HPP
#include <string>
#include <tuple>
#include "Viper/Expressions/Expression.hpp"
#include "Viper/Row.hpp"

namespace Viper {

  /*! \brief Represents an SQL UPDATE statement.
      \tparam R The type row to update.
   */
  template<typename R>
  class UpdateStatement {
    public:

      //! The type of row to update.
      using Row = R;

      //! Type used to represent the set clause.
      using SetClause = std::tuple<std::string_view, Expression>;

      //! Constructs an update statement.
      /*!
        \param row The type of row to update.
        \param table The name of the table to update.
        \param set The set clause.
      */
      UpdateStatement(Row row, std::string table, SetClause set);

      //! Returns the type of row to update.
      const Row& get_row() const;

      //! Returns the name of the table to update.
      const std::string& get_table() const;

      //! Returns the set clause.
      const SetClause& get_set() const;

    private:
      Row m_row;
      std::string m_table;
      SetClause m_set;
  };

  //! Builds an update statement.
  /*!
    \param row The type of row to update.
    \param table The name of the table to update.
    \param set The set clause.
  */
  template<typename R>
  auto update(R row, std::string table,
      typename UpdateStatement<R>::SetClause set) {
    return UpdateStatement(std::move(row), std::move(table), std::move(set));
  }

  template<typename R>
  UpdateStatement<R>::UpdateStatement(Row row, std::string table, SetClause set)
      : m_row(std::move(row)),
        m_table(std::move(table)),
        m_set(std::move(set)) {}

  template<typename R>
  const typename UpdateStatement<R>::Row& UpdateStatement<R>::get_row() const {
    return m_row;
  }

  template<typename R>
  const std::string& UpdateStatement<R>::get_table() const {
    return m_table;
  }

  template<typename R>
  const typename UpdateStatement<R>::SetClause&
      UpdateStatement<R>::get_set() const {
    return m_set;
  }
}

#endif
