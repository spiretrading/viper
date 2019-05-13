#ifndef VIPER_DATE_TIME_DATA_TYPE_HPP
#define VIPER_DATE_TIME_DATA_TYPE_HPP
#include <array>
#include <cstdint>
#include <ctime>
#include <string>
#include "Viper/DataTypes/DataType.hpp"

namespace Viper {

  /** Stores a DateTime. */
  class DateTime {
    public:

      //! The number of ticks per second used by this type.
      static constexpr auto TICKS_PER_SECOND = 1000;

      //! Constructs an uninitialized DateTime.
      DateTime();

      //! Constructs a DateTime to a specified point in time.
      /*!
        \param year The year to represent.
        \param month The month [0-11].
        \param day The day of month [1-31].
        \param hour The number of hours since midnight [0-23].
        \param minute The number of minutes since the hour [0-59].
        \param second The number of seconds since the minute [0-59].
        \param milliseconds The number of milliseconds since the second [0-999].
      */
      DateTime(int year, int month, int day, int hour, int minute, int second,
        int milliseconds);

      //! Returns the number of ticks used to represent this DateTime.
      std::uint64_t get_ticks() const;

    private:
      std::uint64_t m_ticks;
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
  /*!
    \param date_time The DateTime to convert.
    \return The string representation suitable for storing in SQL.
  */
  inline std::string to_string(DateTime date_time) {
    auto buffer = std::string();
    buffer = '\'';
    buffer.resize(21);
    auto delta = std::time_t(
      date_time.get_ticks() / DateTime::TICKS_PER_SECOND);
    std::strftime(&buffer[1], 20, "%Y-%m-%d %H:%M:%S", std::gmtime(&delta));
    auto fraction = date_time.get_ticks() - DateTime::TICKS_PER_SECOND * delta;
    buffer.pop_back();
    if(fraction != 0) {
      buffer += '.';
      buffer += std::to_string(fraction);
    }
    buffer += '\'';
    return buffer;
  }

  inline DateTime::DateTime()
      : m_ticks(0) {}

  inline DateTime::DateTime(int year, int month, int day, int hour, int minute,
      int second, int milliseconds) {
    auto tm = std::tm();
    tm.tm_year = year - 1900;
    tm.tm_mon = month;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    tm.tm_isdst = -1;
#ifdef _MSC_VER
    m_ticks = TICKS_PER_SECOND * static_cast<std::uint64_t>(
      ::_mkgmtime(&tm)) + milliseconds;
#else
    m_ticks = TICKS_PER_SECOND * static_cast<std::uint64_t>(
      std::timegm(&tm)) + milliseconds;
#endif
  }

  inline std::uint64_t DateTime::get_ticks() const {
    return m_ticks;
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
