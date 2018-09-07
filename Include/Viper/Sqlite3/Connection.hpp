#ifndef VIPER_SQLITE3_CONNECTION_HPP
#define VIPER_SQLITE3_CONNECTION_HPP
#include <string>
#include <sqlite3.h>
#include "Viper/ConnectException.hpp"
#include "Viper/CreateTableStatement.hpp"
#include "Viper/DeleteStatement.hpp"
#include "Viper/ExecuteException.hpp"
#include "Viper/InsertRangeStatement.hpp"
#include "Viper/SelectStatement.hpp"
#include "Viper/Sqlite3/DataTypeName.hpp"
#include "Viper/Sqlite3/QueryBuilder.hpp"

namespace Viper::Sqlite3 {

  //! Represents a connection to an SQLite database.
  class Connection {
    public:

      //! Constructs a connection to an SQLite database.
      /*!
        \param path The path to the database.
      */
      Connection(std::string path);

      //! Moves a SQLite connection.
      Connection(Connection&& connection);

      ~Connection();

      //! Executes a create table statement.
      /*!
        \param s The statement to execute.
      */
      template<typename T>
      void execute(const CreateTableStatement<T>& s);

      //! Executes a delete statement.
      /*!
        \param s The statement to execute.
      */
      void execute(const DeleteStatement& s);

      //! Executes an insert range statement.
      /*!
        \param s The statement to execute.
      */
      template<typename T, typename B, typename E>
      void execute(const InsertRangeStatement<T, B, E>& s);

      //! Executes a select statement.
      /*!
        \param s The statement to execute.
      */
      template<typename T, typename D>
      void execute(const SelectStatement<T, D>& s);

      //! Opens a connection to the SQLite database.
      void open();

      //! Closes the connection to the SQLite database.
      void close();

    private:
      std::string m_path;
      ::sqlite3* m_handle;

      Connection(const Connection&) = delete;
      Connection& operator =(const Connection&) = delete;
  };

  inline Connection::Connection(std::string path)
      : m_path(std::move(path)),
        m_handle(nullptr) {}

  inline Connection::Connection(Connection&& connection)
      : m_path(std::move(connection.m_path)),
        m_handle(connection.m_handle) {
    connection.m_handle = nullptr;
  }

  inline Connection::~Connection() {
    close();
  }

  template<typename T>
  void Connection::execute(const CreateTableStatement<T>& s) {
    std::string query;
    build_query(s, query);
    if(query.empty()) {
      return;
    }
    char* error;
    auto result = ::sqlite3_exec(m_handle, query.c_str(), nullptr, nullptr,
      &error);
    if(result != SQLITE_OK) {
      std::string err = error;
      ::sqlite3_free(error);
      throw ExecuteException(err);
    }
  }

  inline void Connection::execute(const DeleteStatement& s) {
    std::string query;
    build_query(s, query);
    if(query.empty()) {
      return;
    }
    char* error;
    auto result = ::sqlite3_exec(m_handle, query.c_str(), nullptr, nullptr,
      &error);
    if(result != SQLITE_OK) {
      std::string err = error;
      ::sqlite3_free(error);
      throw ExecuteException(err);
    }
  }

  template<typename T, typename B, typename E>
  void Connection::execute(const InsertRangeStatement<T, B, E>& s) {
    std::string query;
    build_query(s, query);
    if(query.empty()) {
      return;
    }
    char* error;
    auto result = ::sqlite3_exec(m_handle, query.c_str(), nullptr, nullptr,
      &error);
    if(result != SQLITE_OK) {
      std::string err = error;
      ::sqlite3_free(error);
      throw ExecuteException(err);
    }
  }

  template<typename T, typename D>
  void Connection::execute(const SelectStatement<T, D>& s) {
    std::string query;
    build_query(s, query);
    if(query.empty()) {
      return;
    }
    struct closure {
      const SelectStatement<T, D>* m_statement;
      typename SelectStatement<T, D>::Destination m_destination;
    };
    closure c{&s, s.get_first()};
    auto callback = [] (void* data, int count, char** values, char** names) {
      auto& c = *reinterpret_cast<closure*>(data);
      typename SelectStatement<T, D>::Row::Type value;
      c.m_statement->get_row().extract(const_cast<const char**>(values), value);
      *(c.m_destination) = std::move(value);
      ++c.m_destination;
      return 0;
    };
    char* error;
    auto result = ::sqlite3_exec(m_handle, query.c_str(), +callback, &c,
      &error);
    if(result != SQLITE_OK) {
      std::string err = error;
      ::sqlite3_free(error);
      throw ExecuteException(err);
    }
  }

  inline void Connection::open() {
    if(m_handle != nullptr) {
      return;
    }
    auto result = ::sqlite3_open(m_path.c_str(), &m_handle);
    if(result != SQLITE_OK) {
      auto message = ::sqlite3_errmsg(m_handle);
      ::sqlite3_close(m_handle);
      m_handle = nullptr;
      throw ConnectException(message);
    }
  }

  inline void Connection::close() {
    if(m_handle == nullptr) {
      return;
    }
    ::sqlite3_close(m_handle);
    m_handle = nullptr;
  }
}

#endif
