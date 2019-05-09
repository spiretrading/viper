#ifndef VIPER_ROLLBACK_STATEMENT_HPP
#define VIPER_ROLLBACK_STATEMENT_HPP

namespace Viper {

  /** SQL statement used to rollback a transaction. */
  class RollbackStatement {};

  //! Builds a rollback statement.
  inline auto rollback() {
    return RollbackStatement();
  }
}

#endif
