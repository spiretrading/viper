#ifndef VIPER_START_TRANSACTION_STATEMENT_HPP
#define VIPER_START_TRANSACTION_STATEMENT_HPP

namespace Viper {

  /** SQL statement used to start a transaction. */
  class StartTransactionStatement {};

  //! Builds a start transaction statement.
  inline auto start_transaction() {
    return StartTransactionStatement();
  }
}

#endif
