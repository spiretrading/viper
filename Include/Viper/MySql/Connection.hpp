#ifndef VIPER_MYSQL_CONNECTION_HPP
#define VIPER_MYSQL_CONNECTION_HPP
#include <string>
#include <mysql.h>
#include "Viper/ConnectException.hpp"
#include "Viper/CreateTableStatement.hpp"
#include "Viper/DeleteStatement.hpp"
#include "Viper/ExecuteException.hpp"
#include "Viper/InsertRangeStatement.hpp"
#include "Viper/SelectStatement.hpp"
#include "Viper/MySql/DataTypeName.hpp"
#include "Viper/MySql/QueryBuilder.hpp"

namespace Viper::MySql {

  //! Represents a connection to a MySQL database.
  class Connection {
    public:

      //! Constructs a connection to a MySQL database.
      /*!
        \param host The host to connect to.
        \param port The connection's port.
        \param username The username to connect.
        \param password The username's password.
        \param database The database to use.
      */
      Connection(std::string host, unsigned int port, std::string username,
        std::string password, std::string database);

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
      std::string m_host;
      unsigned int m_port;
      std::string m_username;
      std::string m_password;
      std::string m_database;
      ::MYSQL* m_handle;

      void execute(const std::string& query);
  };

  inline Connection::Connection(std::string host, unsigned int port,
      std::string username, std::string password, std::string database)
      : m_host(std::move(host)),
        m_port(port),
        m_username(std::move(username)),
        m_password(std::move(password)),
        m_database(std::move(database)),
        m_handle(nullptr) {}

  inline Connection::~Connection() {
    close();
  }

  template<typename T>
  void Connection::execute(const CreateTableStatement<T>& s) {
    std::string query;
    build_query(s, query);
    execute(query);
  }

  inline void Connection::execute(const DeleteStatement& s) {
    std::string query;
    build_query(s, query);
    execute(query);
  }

  template<typename T, typename B, typename E>
  void Connection::execute(const InsertRangeStatement<T, B, E>& s) {
    constexpr auto MAX_WRITES = std::size_t(300);
    auto count = std::distance(s.get_begin(), s.get_end());
    execute("START TRANSACTION;");
    auto i = s.get_begin();
    while(count != 0) {
      auto sub_count = std::min<std::size_t>(MAX_WRITES, count);
      auto e = i;
      std::advance(e, sub_count);
      auto sub_range = insert(s.get_row(), s.get_table(), i, e);
      std::string query;
      build_query(sub_range, query);
      execute(query);
      i = e;
      count -= sub_count;
    }
    execute("COMMIT;");
  }

  template<typename T, typename D>
  void Connection::execute(const SelectStatement<T, D>& s) {
    std::string query;
    build_query(s, query);
    if(query.empty()) {
      return;
    }
    auto result = ::mysql_query(m_handle, query.c_str());
    if(result != 0) {
      throw ExecuteException(::mysql_error(m_handle));
    }
    auto rows = ::mysql_store_result(m_handle);
    if(rows == nullptr) {
      throw ExecuteException(::mysql_error(m_handle));
    }
    auto count = ::mysql_num_fields(rows);
    auto destination = s.get_first();
    while(auto row = ::mysql_fetch_row(rows)) {
      typename SelectStatement<T, D>::Row::Type value;
      s.get_row().extract(const_cast<const char**>(row), value);
      destination = std::move(value);
      ++destination;
    }
    ::mysql_free_result(rows);
  }

  inline void Connection::open() {
    if(m_handle != nullptr) {
      return;
    }
    m_handle = ::mysql_init(nullptr);
    if(m_handle == nullptr) {
      throw ConnectException("Unable to allocate MySQL connection.");
    }
    auto result = ::mysql_real_connect(m_handle, m_host.c_str(),
      m_username.c_str(), m_password.c_str(), m_database.c_str(), m_port,
      nullptr, CLIENT_MULTI_STATEMENTS);
    if(result == nullptr) {
      auto error = std::string(::mysql_error(m_handle));
      ::mysql_close(m_handle);
      m_handle = nullptr;
      throw ConnectException(error);
    }
  }

  inline void Connection::close() {
    if(m_handle == nullptr) {
      return;
    }
    ::mysql_close(m_handle);
    m_handle = nullptr;
  }

  inline void Connection::execute(const std::string& query) {
    if(query.empty()) {
      return;
    }
    if(::mysql_query(m_handle, query.c_str()) != 0) {
      throw ExecuteException(::mysql_error(m_handle));
    }
    while(true) {
      auto result = ::mysql_store_result(m_handle);
      if (result != nullptr) {
        ::mysql_free_result(result);
      } else if(::mysql_field_count(m_handle) != 0) {
        throw ExecuteException(::mysql_error(m_handle));
      }
      auto next_result = ::mysql_next_result(m_handle);
      if(next_result < 0) {
        break;
      } else if(next_result > 0) {
        throw ExecuteException(::mysql_error(m_handle));
      }
    }
  }
}

#endif
