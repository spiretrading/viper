#ifndef VIPER_SQLITE3_QUERY_BUILDER_HPP
#define VIPER_SQLITE3_QUERY_BUILDER_HPP
#include <string>
#include "viper/create_table_statement.hpp"
#include "viper/insert_range_statement.hpp"
#include "viper/select_clause.hpp"
#include "viper/select_statement.hpp"
#include "viper/sqlite/data_type_name.hpp"

namespace viper::sqlite3 {
namespace details {
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
  void build_query(const create_table_statement<T>& statement,
      std::string& query) {
    query += "BEGIN;CREATE TABLE ";
    if(statement.get_exists_flag()) {
      query += "IF NOT EXISTS ";
    }
    query += statement.get_name() + '(';
    details::append_list(statement.get_table().get_columns(), query,
      [] (auto& column, auto& query) {
        query += column.m_name + ' ' + get_name(*column.m_type);
        if(!column.m_is_nullable) {
          query += " NOT NULL";
        }
      });
    if(!statement.get_table().get_indexes().empty() &&
        statement.get_table().get_indexes().front().m_is_primary) {
      query += ",PRIMARY KEY(";
      details::append_list(
        statement.get_table().get_indexes().front().m_columns, query);
      query += ')';
    }
    query += ");";
    for(auto& current_index : statement.get_table().get_indexes()) {
      if(current_index.m_is_primary) {
        continue;
      }
      if(current_index.m_is_unique) {
        query += "CREATE UNIQUE INDEX";
      } else {
        query += "CREATE INDEX";
      }
      query += " IF NOT EXISTS " + current_index.m_name + " ON " +
        statement.get_name() + "(";
      details::append_list(current_index.m_columns, query);
      query += ");";
    }
    query += "COMMIT;";
  }

  //! Builds an insert range query statement.
  /*!
    \param statement The statement to build.
    \param query The string to store the query in.
  */
  template<typename T, typename B, typename E>
  void build_query(const insert_range_statement<T, B, E>& statement,
      std::string& query) {
    if(statement.get_begin() == statement.get_end()) {
      return;
    }
    query += "INSERT INTO ";
    query += statement.get_into_table();
    query += " (";
    details::append_list(statement.get_from_table().get_columns(), query,
      [] (auto& column, auto& query) {
        query += column.m_name;
      });
    query += ") VALUES ";
    details::append_list(statement.get_begin(), statement.get_end(), query,
      [] (auto& column, auto& query) {
        query += '(';
        auto prepend_comma = false;
        for(int j = 0; j < static_cast<int>(
            statement.get_from_table().get_columns().size()); ++j) {
          if(prepend_comma) {
            query += ',';
          }
          prepend_comma = true;
          statement.get_from_table().append_value(*i, j, query);
        }
        query += ')';
      });
    query += ';';
  }

  //! Builds a select query clause.
  /*!
    \param clause The clause to build.
    \param query The string to store the query in.
  */
  inline void build_query(const select_clause& clause, std::string& query) {
    query += "SELECT ";
    details::append_list(clause.get_columns(), query);
    query += " FROM ";
    auto& from = clause.get_from();
    if(auto t = std::get_if<std::string>(&from)) {
      query += *t;
    } else if(auto t = std::get_if<std::shared_ptr<select_clause>>(&from)) {
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
        details::append_list(clause.get_order()->m_columns, query);
        query += ')';
      }
      if(clause.get_order()->m_order == order::ASC) {
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
  void build_query(const select_statement<T, D>& statement,
      std::string& query) {
    build_query(statement.get_clause(), query);
    query += ';';
  }
}

#endif
