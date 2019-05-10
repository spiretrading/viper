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
  void transaction(Connection& connection, F&& transaction) {
    connection.execute(start_transaction());
    try {
      transaction();
    } catch(...) {
      connection.execute(rollback());
      throw;
    }
    connection.execute(commit());
  }
}

#endif
