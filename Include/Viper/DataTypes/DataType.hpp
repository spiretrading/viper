#ifndef VIPER_DATA_TYPE_HPP
#define VIPER_DATA_TYPE_HPP
#include <memory>
#include "Viper/DataTypes/DataTypeVisitor.hpp"

namespace Viper {

  /** Base class used to represent an SQL data type. */
  class DataType {
    public:
      virtual ~DataType() = default;

      //! Tests if two objects represent the same data type.
      bool operator ==(const DataType& right) const;

      //! Tests if two objects represent distinct data types.
      bool operator !=(const DataType& right) const;

      //! Returns a copy of this data type.
      virtual std::unique_ptr<DataType> clone() const = 0;

      //! Applies a data type visitor to this instance.
      /*!
        \param visitor The visitor to apply.
      */
      virtual void apply(DataTypeVisitor& visitor) const = 0;

    protected:

      //! Constructs an SQL data type.
      DataType() = default;

      //! Copies an SQL data type.
      DataType(const DataType&) = default;

      //! Assigns to this SQL data type.
      DataType& operator =(const DataType&) = default;

      //! Implements the equality test.
      virtual bool is_equal(const DataType& right) const = 0;
  };

  inline bool DataType::operator ==(const DataType& right) const {
    if(this == &right) {
      return true;
    } else if(typeid(*this) != typeid(right)) {
      return false;
    }
    return is_equal(right);
  }

  inline bool DataType::operator !=(const DataType& right) const {
    return !(*this == right);
  }
}

#endif
