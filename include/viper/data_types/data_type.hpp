#ifndef VIPER_DATA_TYPE_HPP
#define VIPER_DATA_TYPE_HPP
#include <memory>
#include "viper/data_types/data_type_visitor.hpp"

namespace viper {

  /** Base class used to represent an SQL data type. */
  class data_type {
    public:
      virtual ~data_type() = default;

      //! Tests if two objects represent the same data type.
      bool operator ==(const data_type& right) const;

      //! Tests if two objects represent distinct data types.
      bool operator !=(const data_type& right) const;

      //! Returns a copy of this data type.
      virtual std::unique_ptr<data_type> clone() const = 0;

      //! Applies a data type visitor to this instance.
      /*!
        \param visitor The visitor to apply.
      */
      virtual void apply(data_type_visitor& visitor) const = 0;

    protected:

      //! Constructs an SQL data type.
      data_type() = default;

      //! Copies an SQL data type.
      data_type(const data_type&) = default;

      //! Assigns to this SQL data type.
      data_type& operator =(const data_type&) = default;

      //! Implements the equality test.
      virtual bool is_equal(const data_type& right) const = 0;
  };

  inline bool data_type::operator ==(const data_type& right) const {
    if(this == &right) {
      return true;
    } else if(typeid(*this) != typeid(right)) {
      return false;
    }
    return is_equal(right);
  }

  inline bool data_type::operator !=(const data_type& right) const {
    return !(*this == right);
  }
}

#endif
