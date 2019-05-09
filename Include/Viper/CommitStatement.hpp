#ifndef VIPER_COMMIT_STATEMENT_HPP
#define VIPER_COMMIT_STATEMENT_HPP

namespace Viper {

  /** SQL statement used to commit a transaction. */
  class CommitStatement {};

  //! Builds a commit statement.
  inline auto commit() {
    return CommitStatement();
  }
}

#endif
