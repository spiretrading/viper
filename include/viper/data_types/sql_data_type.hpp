#ifndef VIPER_SQL_DATA_TYPE_HPP
#define VIPER_SQL_DATA_TYPE_HPP

namespace viper {

  /** Base class used to represent an SQL data type. */
  class sql_data_type {
    public:
      virtual ~sql_data_type() = default;

      //! Tests if two objects represent the same data type.
      bool operator ==(const sql_data_type& right) const;

      //! Tests if two objects represent distinct data types.
      bool operator !=(const sql_data_type& right) const;

    protected:

      //! Implements the equality test.
      virtual bool operator is_equal(const sql_data_type& right) const = 0;

    private:
      sql_data_type(const sql_data_type&) = delete;
      sql_data_type& operator(const sql_data_type&) = delete;
  };

  inline bool sql_data_type::operator ==(const sql_data_type& right) const {
    return typeid(*this) == typeid(*right) && is_equal(right);
  }

  inline bool sql_data_type::operator !=(const sql_data_type& right) const {
    return !(*this == right);
  }
}

#endif
