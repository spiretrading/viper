#ifndef VIPER_SQLITE_CONNECTION_HPP
#define VIPER_SQLITE_CONNECTION_HPP
#include <string>
#include <sqlite3.h>
#include "viper/connect_exception.hpp"
#include "viper/create_table_statement.hpp"
#include "viper/execute_exception.hpp"
#include "viper/insert_range_statement.hpp"
#include "viper/select_statement.hpp"
#include "viper/sqlite/data_type_name.hpp"

namespace viper::sqlite3 {

  //! Represents a connection to an SQL database.
  class connection {
    public:

      //! Constructs a connection to an SQLite database.
      /*!
        \param path The path to the database.
      */
      connection(std::string path);

      ~connection();

      //! Executes a create table statement.
      /*!
        \param s The statement to execute.
      */
      template<typename T>
      void execute(const create_table_statement<T>& s);

      //! Executes an insert range statement.
      /*!
        \param s The statement to execute.
      */
      template<typename T, typename B, typename E>
      void execute(const insert_range_statement<T, B, E>& s);

      //! Executes a select statement.
      /*!
        \param s The statement to execute.
      */
      template<typename T, typename D>
      void execute(const select_statement<T, D>& s);

      //! Opens a connection to the SQLite database.
      void open();

      //! Closes the connection to the SQLite database.
      void close();

    private:
      std::string m_path;
      ::sqlite3* m_handle;
  };

  inline connection::connection(std::string path)
      : m_path(std::move(path)),
        m_handle(nullptr) {}

  inline connection::~connection() {
    close();
  }

  template<typename T>
  void connection::execute(const create_table_statement<T>& s) {
    std::string query = "CREATE TABLE ";
    if(s.get_exists_flag()) {
      query += "IF NOT EXISTS ";
    }
    query += s.get_name() + '(';
    auto prepend_comma = false;
    for(auto& column : s.get_table().get_columns()) {
      if(prepend_comma) {
        query += ',';
      }
      prepend_comma = true;
      query += column.m_name;
      query += ' ';
      query += get_name(*column.m_type);
    }
    query += ");";
    char* error;
    auto result = ::sqlite3_exec(m_handle, query.c_str(), nullptr, nullptr,
      &error);
    if(result != SQLITE_OK) {
      ::sqlite3_free(error);
      throw execute_exception(error);
    }
  }

  template<typename T, typename B, typename E>
  void connection::execute(const insert_range_statement<T, B, E>& s) {
    if(s.get_begin() == s.get_end()) {
      return;
    }
    std::string query = "INSERT INTO ";
    query += s.get_into_table();
    query += " (";
    auto prepend_comma = false;
    for(auto& column : s.get_from_table().get_columns()) {
      if(prepend_comma) {
        query += ',';
      }
      prepend_comma = true;
      query += column.m_name;
    }
    query += ") VALUES ";
    prepend_comma = false;
    for(auto i = s.get_begin(); i != s.get_end(); ++i) {
      if(prepend_comma) {
        query += ',';
      }
      prepend_comma = true;
      query += '(';
      auto prepend_value_comma = false;
      for(int j = 0;
          j < static_cast<int>(s.get_from_table().get_columns().size()); ++j) {
        if(prepend_value_comma) {
          query += ',';
        }
        prepend_value_comma = true;
        s.get_from_table().append_value(*i, j, query);
      }
      query += ')';
    }
    query += ';';
    char* error;
    auto result = ::sqlite3_exec(m_handle, query.c_str(), nullptr, nullptr,
      &error);
    if(result != SQLITE_OK) {
      ::sqlite3_free(error);
      throw execute_exception(error);
    }
  }

  template<typename T, typename D>
  void connection::execute(const select_statement<T, D>& s) {
    std::string query = "SELECT ";
    auto prepend_comma = false;
    for(auto& column : s.get_result_table().get_columns()) {
      if(prepend_comma) {
        query += ',';
      }
      prepend_comma = true;
      query += column.m_name;
    }
    query += " FROM ";
    query += s.get_from_table();
    query += ';';
    struct closure {
      const select_statement<T, D>* m_statement;
      typename select_statement<T, D>::destination m_destination;
    };
    closure c{&s, s.get_first()};
    auto callback = [] (void* data, int count, char** values, char** names) {
      auto& c = *reinterpret_cast<closure*>(data);
      typename select_statement<T, D>::result_table::type value;
      c.m_statement->get_result_table().extract(
        const_cast<const char**>(values), value);
      c.m_destination = std::move(value);
      ++c.m_destination;
      return 0;
    };
    char* error;
    auto result = ::sqlite3_exec(m_handle, query.c_str(), +callback, &c,
      &error);
    if(result != SQLITE_OK) {
      ::sqlite3_free(error);
      throw execute_exception(error);
    }
  }

  inline void connection::open() {
    if(m_handle != nullptr) {
      return;
    }
    auto result = ::sqlite3_open(m_path.c_str(), &m_handle);
    if(result != SQLITE_OK) {
      auto message = ::sqlite3_errmsg(m_handle);
      ::sqlite3_close(m_handle);
      m_handle = nullptr;
      throw connect_exception(message);
    }
  }

  inline void connection::close() {
    if(m_handle == nullptr) {
      return;
    }
    ::sqlite3_close(m_handle);
    m_handle = nullptr;
  }
}

#endif
