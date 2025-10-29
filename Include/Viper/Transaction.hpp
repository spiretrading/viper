#ifndef VIPER_TRANSACTION_HPP
#define VIPER_TRANSACTION_HPP
#include "Viper/CommitStatement.hpp"
#include "Viper/RollbackStatement.hpp"
#include "Viper/StartTransactionStatement.hpp"

namespace Viper {

  /** Executes a transactional series of statements.
   * @param connection - The SQL connection making the transaction.
   * @param transaction - A callable with signature void () representing the
   *        series of statements to execute.
   */
  template<typename Connection, typename F>
  decltype(auto) transaction(Connection& connection, F&& f) {
    struct CommitGuard {
      Connection* m_connection;
      int m_exception_count;

      CommitGuard(Connection& connection)
        : m_connection(&connection),
          m_exception_count(std::uncaught_exceptions()) {}

      ~CommitGuard() {
        if(std::uncaught_exceptions() == m_exception_count) {
          m_connection->execute(commit());
        }
      }
    };
    connection.execute(start_transaction());
    auto guard = CommitGuard(connection);
    try {
      return std::forward<F>(f)();
    } catch(...) {
      connection.execute(rollback());
      throw;
    }
  }
}

#endif
