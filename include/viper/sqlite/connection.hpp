#ifndef VIPER_SQLITE_CONNECTION_HPP
#define VIPER_SQLITE_CONNECTION_HPP
#include <string>
#include <string_view>
#include <sqlite3.h>
#include "viper/connect_exception.hpp"
#include "viper/create_table.hpp"

namespace viper {
namespace sqlite3 {

  //! Represents a connection to an SQL database.
  class connection {
    public:

      //! Constructs a connection to an SQLite database.
      /*!
        \param path The path to the database.
      */
      connection(std::string_view path);

      ~connection();

      //! Executes a create table statement.
      /*!
        \param s The statement to execute.
      */
      template<typename T>
      void execute(const create_table_statement<T>& s);

      //! Opens a connection to the SQLite database.
      void open();

      //! Closes the connection to the SQLite database.
      void close();

    private:
      std::string m_path;
      ::sqlite3* m_handle;
  };

  inline connection::connection(std::string_view path)
      : m_path(path),
        m_handle(nullptr) {}

  inline connection::~connection() {
    close();
  }

  template<typename T>
  void connection::execute(const create_table_statement<T>& s) {
    std::string query = "CREATE TABLE ";
    query += s.get_name() + '(';
    for(auto& column : s.get_table().
    query += ')';
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
}

#endif
