#ifndef VIPER_DATE_TIME_DATA_TYPE_HPP
#define VIPER_DATE_TIME_DATA_TYPE_HPP
#include <string>
#include "Viper/DataTypes/DataType.hpp"

namespace Viper {

  /** Stores a DateTime. */
  class DateTime {
  };

  /** Represents an SQL date/time data type. */
  class DateTimeDataType final : public DataType {
    public:
      std::unique_ptr<DataType> clone() const override;

      void apply(DataTypeVisitor& visitor) const override;

    protected:
      virtual bool is_equal(const DataType& right) const override;
  };

  //! Date/time data type.
  static inline auto date_time = DateTimeDataType();

  //! Converts a DateTime to a string.
  inline std::string to_string(DateTime date_time) {
    return "";
  }

  inline std::unique_ptr<DataType> DateTimeDataType::clone() const {
    return std::make_unique<DateTimeDataType>(*this);
  }

  inline void DateTimeDataType::apply(DataTypeVisitor& visitor) const {
    visitor.visit(*this);
  }

  inline bool DateTimeDataType::is_equal(const DataType& right) const {
    return true;
  }

  inline void DataTypeVisitor::visit(const DateTimeDataType& node) {
    visit(static_cast<const DataType&>(node));
  }
}

#endif
