#ifndef VIPER_SQLITE3_QUERY_BUILDER_HPP
#define VIPER_SQLITE3_QUERY_BUILDER_HPP
#include <string>
#include "viper/create_table_statement.hpp"
#include "viper/insert_range_statement.hpp"
#include "viper/select_statement.hpp"
#include "viper/sqlite/data_type_name.hpp"

namespace viper::sqlite3 {

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
    auto prepend_comma = false;
    for(auto& column : statement.get_table().get_columns()) {
      if(prepend_comma) {
        query += ',';
      }
      prepend_comma = true;
      query += column.m_name;
      query += ' ';
      query += get_name(*column.m_type);
      if(!column.m_is_nullable) {
        query += " NOT NULL";
      }
    }
    if(!statement.get_table().get_indexes().empty() &&
        statement.get_table().get_indexes().front().m_is_primary) {
      query += ",PRIMARY KEY(";
      auto prepend_comma = false;
      auto& primary = statement.get_table().get_indexes().front();
      for(auto& c : primary.m_columns) {
        if(prepend_comma) {
          query += ',';
        }
        prepend_comma = true;
        query += c;
      }
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
      auto prepend_comma = false;
      for(auto& c : current_index.m_columns) {
        if(prepend_comma) {
          query += ',';
        }
        prepend_comma = true;
        query += c;
      }
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
    auto prepend_comma = false;
    for(auto& column : statement.get_from_table().get_columns()) {
      if(prepend_comma) {
        query += ',';
      }
      prepend_comma = true;
      query += column.m_name;
    }
    query += ") VALUES ";
    prepend_comma = false;
    for(auto i = statement.get_begin(); i != statement.get_end(); ++i) {
      if(prepend_comma) {
        query += ',';
      }
      prepend_comma = true;
      query += '(';
      auto prepend_value_comma = false;
      for(int j = 0; j < static_cast<int>(
          statement.get_from_table().get_columns().size()); ++j) {
        if(prepend_value_comma) {
          query += ',';
        }
        prepend_value_comma = true;
        statement.get_from_table().append_value(*i, j, query);
      }
      query += ')';
    }
    query += ';';
  }

  //! Builds a select query statement.
  /*!
    \param statement The statement to build.
    \param query The string to store the query in.
  */
  template<typename T, typename D>
  void build_query(const select_statement<T, D>& statement,
      std::string& query) {
    query += "SELECT ";
    auto prepend_comma = false;
    for(auto& column : statement.get_result_table().get_columns()) {
      if(prepend_comma) {
        query += ',';
      }
      prepend_comma = true;
      query += column.m_name;
    }
    query += " FROM ";
    query += statement.get_from_table();
    if(statement.get_where() != std::nullopt) {
      query += " WHERE ";
      statement.get_where()->append_query(query);
    }
    query += ';';
  }
}

#endif
