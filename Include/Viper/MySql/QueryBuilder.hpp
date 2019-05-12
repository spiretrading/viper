#ifndef VIPER_MYSQL_QUERY_BUILDER_HPP
#define VIPER_MYSQL_QUERY_BUILDER_HPP
#include <string>
#include <vector>
#include "Viper/CommitStatement.hpp"
#include "Viper/CreateTableStatement.hpp"
#include "Viper/DeleteStatement.hpp"
#include "Viper/InsertRangeStatement.hpp"
#include "Viper/MySql/DataTypeName.hpp"
#include "Viper/RollbackStatement.hpp"
#include "Viper/SelectClause.hpp"
#include "Viper/SelectStatement.hpp"
#include "Viper/StartTransactionStatement.hpp"

namespace Viper::MySql {
namespace Details {
  template<typename B, typename E, typename F>
  void append_list(B b, E e, std::string& query, F&& f) {
    auto prepend_comma = false;
    for(auto i = b; i != e; ++i) {
      if(prepend_comma) {
        query += ',';
      }
      prepend_comma = true;
      f(*i, query);
    }
  }

  template<typename T, typename F>
  void append_list(const T& list, std::string& query, F&& f) {
    append_list(list.begin(), list.end(), query, f);
  }

  template<typename T>
  void append_list(const T& list, std::string& query) {
    return append_list(list, query,
      [] (auto& item, std::string& query) {
        query += item;
      });
  }
}

  //! Builds a create table query statement.
  /*!
    \param statement The statement to build.
    \param query The string to store the query in.
  */
  template<typename T>
  void build_query(const CreateTableStatement<T>& statement,
      std::string& query) {
    query += "BEGIN;CREATE TABLE ";
    if(statement.get_exists_flag()) {
      query += "IF NOT EXISTS ";
    }
    query += statement.get_name() + '(';
    Details::append_list(statement.get_row().get_columns(), query,
      [] (auto& column, auto& query) {
        query += column.m_name + ' ' + get_name(*column.m_type);
        if(!column.m_is_nullable) {
          query += " NOT NULL";
        }
      });
    for(auto& index : statement.get_row().get_indexes()) {
      if(index.m_is_primary) {
        query += ",PRIMARY KEY(";
        Details::append_list(index.m_columns, query);
        query += ')';
      } else if(index.m_is_unique) {
        query += ",UNIQUE KEY " + index.m_name + "(";
        Details::append_list(index.m_columns, query);
        query += ')';
      } else {
        query += ",KEY " + index.m_name + "(";
        Details::append_list(index.m_columns, query);
        query += ')';
      }
    }
    query += ");";
    query += "COMMIT;";
  }

  //! Builds a delete query statement.
  /*!
    \param statement The statement to build.
    \param query The string to store the query in.
  */
  inline void build_query(const DeleteStatement& statement,
      std::string& query) {
    if(statement.get_where().has_value()) {
      query += "DELETE FROM " + statement.get_table() + " WHERE ";
      statement.get_where()->append_query(query);
    } else {
      query += "TRUNCATE TABLE " + statement.get_table();
    }
    query += ';';
  }

  //! Builds an insert range query statement.
  /*!
    \param statement The statement to build.
    \param query The string to store the query in.
  */
  template<typename T, typename B, typename E>
  void build_query(const InsertRangeStatement<T, B, E>& statement,
      std::string& query) {
    if(statement.get_begin() == statement.get_end() ||
        statement.get_row().get_columns().empty()) {
      return;
    }
    query += "INSERT INTO ";
    query += statement.get_table();
    query += " (";
    Details::append_list(statement.get_row().get_columns(), query,
      [] (auto& column, auto& query) {
        query += column.m_name;
      });
    query += ") VALUES ";
    Details::append_list(statement.get_begin(), statement.get_end(), query,
      [&] (auto& column, auto& query) {
        query += '(';
        auto prepend_comma = false;
        for(auto i = 0; i <
            static_cast<int>(statement.get_row().get_columns().size()); ++i) {
          if(prepend_comma) {
            query += ',';
          }
          prepend_comma = true;
          statement.get_row().append_value(column, i, query);
        }
        query += ')';
      });
    query += ';';
  }

  //! Builds an update statement.
  /*!
    \param statement The statement to build.
    \param query The string to store the query in.
  */
  inline void build_query(const UpdateStatement& statement,
      std::string& query) {
    query += "UPDATE ";
    query += statement.get_table();
    query += " SET ";
    query += std::get<0>(statement.get_set());
    query += " = ";
    std::get<1>(statement.get_set()).append_query(query);
    query += ';';
  }

  //! Builds a select query clause.
  /*!
    \param clause The clause to build.
    \param query The string to store the query in.
  */
  inline void build_query(const SelectClause& clause, std::string& query) {
    query += "SELECT ";
    Details::append_list(clause.get_columns(), query);
    query += " FROM ";
    auto& from = clause.get_from();
    if(auto t = std::get_if<std::string>(&from)) {
      query += *t;
    } else if(auto t = std::get_if<std::shared_ptr<SelectClause>>(&from)) {
      query += '(';
      build_query(**t, query);
      query += ") AS alias";
    }
    if(clause.get_where() != std::nullopt) {
      query += " WHERE ";
      clause.get_where()->append_query(query);
    }
    if(clause.get_order() != std::nullopt &&
        !clause.get_order()->m_columns.empty()) {
      query += " ORDER BY ";
      if(clause.get_order()->m_columns.size() == 1) {
        query += clause.get_order()->m_columns.front();
      } else {
        query += '(';
        Details::append_list(clause.get_order()->m_columns, query);
        query += ')';
      }
      if(clause.get_order()->m_order == Order::ASC) {
        query += " ASC";
      } else {
        query += " DESC";
      }
    }
    if(clause.get_limit() != std::nullopt) {
      query += " LIMIT ";
      query += std::to_string(clause.get_limit()->m_value);
    }
  }

  //! Builds a select query statement.
  /*!
    \param statement The statement to build.
    \param query The string to store the query in.
  */
  template<typename T, typename D>
  void build_query(const SelectStatement<T, D>& statement,
      std::string& query) {
    build_query(statement.get_clause(), query);
    query += ';';
  }

  //! Builds a start transaction statement.
  /*!
    \param statement The statement to build.
    \param query The string to store the query in.
  */
  inline void build_query(const StartTransactionStatement& statement,
      std::string& query) {
    query += "BEGIN;";
  }

  //! Builds a commit statement.
  /*!
    \param statement The statement to build.
    \param query The string to store the query in.
  */
  inline void build_query(const CommitStatement& statement,
      std::string& query) {
    query += "COMMIT;";
  }

  //! Builds a rollback statement.
  /*!
    \param statement The statement to build.
    \param query The string to store the query in.
  */
  inline void build_query(const RollbackStatement& statement,
      std::string& query) {
    query += "ROLLBACK;";
  }
}

#endif
