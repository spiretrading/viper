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
#include "Viper/UpdateStatement.hpp"
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

      //! Moves a MySQL connection.
      Connection(Connection&& connection);

      ~Connection();

      //! Tests if a table exists.
      /*!
        \param name The name of the table.
        \return <code>true</code> iff the table exists.
      */
      bool has_table(std::string_view name);

      //! Executes a raw SQL query.
      /*!
        \param statement The statement to execute.
      */
      void execute(std::string_view statement);

      //! Executes a create table statement.
      /*!
        \param statement The statement to execute.
      */
      template<typename T>
      void execute(const CreateTableStatement<T>& statement);

      //! Executes a delete statement.
      /*!
        \param statement The statement to execute.
      */
      void execute(const DeleteStatement& statement);

      //! Executes an insert range statement.
      /*!
        \param statement The statement to execute.
      */
      template<typename T, typename B, typename E>
      void execute(const InsertRangeStatement<T, B, E>& statement);

      //! Executes an update statement.
      /*!
        \param statement The statement to execute.
      */
      template<typename R>
      void execute(const UpdateStatement<R>& statement);

      //! Executes a select statement.
      /*!
        \param statement The statement to execute.
      */
      template<typename T, typename D>
      void execute(const SelectStatement<T, D>& statement);

      //! Starts a transaction.
      /*!
        \param statement The statement to execute.
      */
      void execute(const StartTransactionStatement& statement);

      //! Commits a transaction.
      /*!
        \param statement The statement to execute.
      */
      void execute(const CommitStatement& statement);

      //! Rolls back a transaction.
      /*!
        \param statement The statement to execute.
      */
      void execute(const RollbackStatement& statement);

      //! Opens a connection to the MySQL database.
      void open();

      //! Closes the connection to the MySQL database.
      void close();

    private:
      std::string m_host;
      unsigned int m_port;
      std::string m_username;
      std::string m_password;
      std::string m_database;
      ::MYSQL* m_handle;

      Connection(const Connection&) = delete;
      Connection& operator =(const Connection&) = delete;
  };

  inline Connection::Connection(std::string host, unsigned int port,
      std::string username, std::string password, std::string database)
      : m_host(std::move(host)),
        m_port(port),
        m_username(std::move(username)),
        m_password(std::move(password)),
        m_database(std::move(database)),
        m_handle(nullptr) {}

  inline Connection::Connection(Connection&& connection)
      : m_host(std::move(connection.m_host)),
        m_port(std::move(connection.m_port)),
        m_username(std::move(connection.m_username)),
        m_password(std::move(connection.m_password)),
        m_database(std::move(connection.m_database)),
        m_handle(connection.m_handle) {
    connection.m_handle = nullptr;
  }

  inline Connection::~Connection() {
    close();
  }

  inline void Connection::execute(std::string_view statement) {
    if(statement.empty()) {
      return;
    }
    if(::mysql_query(m_handle, statement.data()) != 0) {
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

  inline bool Connection::has_table(std::string_view name) {
    auto escaped_name = std::string();
    escape(name, escaped_name);
    auto query = std::string("SHOW TABLES IN " + m_database + " LIKE " +
      escaped_name);
    auto result = ::mysql_query(m_handle, query.c_str());
    if(result != 0) {
      throw ExecuteException(::mysql_error(m_handle));
    }
    auto rows = ::mysql_store_result(m_handle);
    if(rows == nullptr) {
      throw ExecuteException(::mysql_error(m_handle));
    }
    auto has_table = (::mysql_fetch_row(rows) != nullptr);
    ::mysql_free_result(rows);
    return has_table;
  }

  template<typename T>
  void Connection::execute(const CreateTableStatement<T>& statement) {
    auto query = std::string();
    build_query(statement, query);
    execute(query);
  }

  inline void Connection::execute(const DeleteStatement& statement) {
    auto query = std::string();
    build_query(statement, query);
    execute(query);
  }

  template<typename T, typename B, typename E>
  void Connection::execute(const InsertRangeStatement<T, B, E>& statement) {
    constexpr auto MAX_WRITES = std::size_t(300);
    auto count = std::distance(statement.get_begin(), statement.get_end());
    execute("START TRANSACTION;");
    auto i = statement.get_begin();
    while(count != 0) {
      auto sub_count = std::min<std::size_t>(MAX_WRITES, count);
      auto e = i;
      std::advance(e, sub_count);
      auto sub_range = insert(statement.get_row(), statement.get_table(), i, e);
      auto query = std::string();
      build_query(sub_range, query);
      execute(query);
      i = e;
      count -= sub_count;
    }
    execute("COMMIT;");
  }

  template<typename R>
  void Connection::execute(const UpdateStatement<R>& statement) {
    auto query = std::string();
    build_query(statement, query);
    execute(query);
  }

  template<typename T, typename D>
  void Connection::execute(const SelectStatement<T, D>& statement) {
    auto query = std::string();
    build_query(statement, query);
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
    auto destination = statement.get_first();
    std::vector<RawColumn> columns;
    columns.reserve(statement.get_row().get_columns().size());
    while(auto row = ::mysql_fetch_row(rows)) {
      columns.clear();
      auto lengths = ::mysql_fetch_lengths(rows);
      for(auto i = 0;
          i != static_cast<int>(statement.get_row().get_columns().size());
          ++i) {
        columns.push_back(RawColumn{
          row[i], static_cast<std::size_t>(lengths[i])});
      }
      auto value = typename SelectStatement<T, D>::Row::Type();
      statement.get_row().extract(columns.data(), value);
      *destination = std::move(value);
      ++destination;
    }
    ::mysql_free_result(rows);
  }

  inline void Connection::execute(const StartTransactionStatement& statement) {
    auto query = std::string();
    build_query(statement, query);
    execute(query);
  }

  inline void Connection::execute(const CommitStatement& statement) {
    auto query = std::string();
    build_query(statement, query);
    execute(query);
  }

  inline void Connection::execute(const RollbackStatement& statement) {
    auto query = std::string();
    build_query(statement, query);
    execute(query);
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
    ::my_bool reconnect = 1;
    ::mysql_options(m_handle, MYSQL_OPT_RECONNECT, &reconnect);
  }

  inline void Connection::close() {
    if(m_handle == nullptr) {
      return;
    }
    ::mysql_close(m_handle);
    m_handle = nullptr;
  }
}

#endif
