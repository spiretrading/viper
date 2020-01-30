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

      //! Tests if a table exists.
      /*!
        \param name The name of the table.
        \return <code>true</code> iff the table exists.
      */
      bool has_table(std::string_view name);

      //! Executes a raw SQL query.
      /*!
        \param s The statement to execute.
      */
      void execute(std::string_view s);

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

      //! Executes an update statement.
      /*!
        \param statement The statement to execute.
      */
      void execute(const UpdateStatement& statement);

      //! Executes an upsert statement.
      /*!
        \param statement The statement to execute.
      */
      template<typename R, typename B, typename E>
      void execute(const UpsertStatement<R, B, E>& statement);

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

  inline bool Connection::has_table(std::string_view name) {
    auto escaped_name = std::string();
    escape(name, escaped_name);
    auto query = "PRAGMA table_info(" + escaped_name + ");";
    auto statement = static_cast<::sqlite3_stmt*>(nullptr);
    auto result = ::sqlite3_prepare_v2(m_handle, query.c_str(), -1, &statement,
      nullptr);
    if(result != SQLITE_OK) {
      throw ExecuteException(::sqlite3_errmsg(m_handle));
    }
    auto has_table = ((result = ::sqlite3_step(statement)) == SQLITE_ROW);
    ::sqlite3_finalize(statement);
    return has_table;
  }

  inline void Connection::execute(std::string_view s) {
    if(s.empty()) {
      return;
    }
    char* error;
    auto result = ::sqlite3_exec(m_handle, s.data(), nullptr, nullptr, &error);
    if(result != SQLITE_OK) {
      std::string err = error;
      ::sqlite3_free(error);
      throw ExecuteException(err);
    }
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
    execute("BEGIN;");
    auto i = s.get_begin();
    while(count != 0) {
      auto sub_count = std::min<std::size_t>(MAX_WRITES, count);
      auto e = i;
      std::advance(e, sub_count);
      auto sub_range = insert(s.get_row(), s.get_table(), i, e);
      auto query = std::string();
      build_query(sub_range, query);
      execute(query);
      i = e;
      count -= sub_count;
    }
    execute("COMMIT;");
  }

  inline void Connection::execute(const UpdateStatement& statement) {
    auto query = std::string();
    build_query(statement, query);
    execute(query);
  }

  template<typename T, typename B, typename E>
  void Connection::execute(const UpsertStatement<T, B, E>& statement) {
    constexpr auto MAX_WRITES = std::size_t(300);
    auto count = std::distance(statement.get_begin(), statement.get_end());
    execute("BEGIN;");
    auto i = statement.get_begin();
    while(count != 0) {
      auto sub_count = std::min<std::size_t>(MAX_WRITES, count);
      auto e = i;
      std::advance(e, sub_count);
      auto sub_range = upsert(statement.get_row(), statement.get_table(), i, e);
      auto query = std::string();
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
    auto statement = static_cast<::sqlite3_stmt*>(nullptr);
    auto result = ::sqlite3_prepare_v2(m_handle, query.c_str(), -1, &statement,
      nullptr);
    if(result != SQLITE_OK) {
      throw ExecuteException(::sqlite3_errmsg(m_handle));
    }
    auto destination = s.get_first();
    std::vector<RawColumn> columns;
    columns.reserve(s.get_row().get_columns().size());
    while((result = ::sqlite3_step(statement)) == SQLITE_ROW) {
      columns.clear();
      for(auto i = 0; i != static_cast<int>(s.get_row().get_columns().size());
          ++i) {
        struct TypeVisitor final : DataTypeVisitor {
          ::sqlite3_stmt* m_statement;
          int m_index;
          std::vector<RawColumn>* m_columns;

          void visit(const BlobDataType& t) override {
            auto data = ::sqlite3_column_blob(m_statement, m_index);
            auto size = ::sqlite3_column_bytes(m_statement, m_index);
            m_columns->push_back(RawColumn{reinterpret_cast<const char*>(data),
              static_cast<std::size_t>(size)});
          }

          void visit(const DataType& t) override {
            auto data = ::sqlite3_column_text(m_statement, m_index);
            m_columns->emplace_back(RawColumn{
              reinterpret_cast<const char*>(data), 0});
          }
        };
        TypeVisitor visitor;
        visitor.m_statement = statement;
        visitor.m_index = i;
        visitor.m_columns = &columns;
        auto& column = s.get_row().get_columns()[i];
        column.m_type->apply(visitor);
      }
      auto value = typename SelectStatement<T, D>::Row::Type();
      s.get_row().extract(columns.data(), value);
      *destination = std::move(value);
      ++destination;
    }
    ::sqlite3_finalize(statement);
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
