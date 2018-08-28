#ifndef VIPER_INTEGER_DATA_TYPE_HPP
#define VIPER_INTEGER_DATA_TYPE_HPP
#include "viper/data_types/data_type.hpp"

namespace Viper {

  /** Represents an SQL integer of some fixed byte size. */
  class IntegerDataType final : public DataType {
    public:

      //! Constructs an SQL integer.
      /*!
        \param is_signed Whether the integer is signed.
        \param size The size (in bytes) of the integer.
      */
      IntegerDataType(bool is_signed, int size);

      //! Returns true iff this is a signed integer.
      bool is_signed() const;

      //! Returns the size (in bytes) of the integer.
      int get_size() const;

      std::unique_ptr<DataType> clone() const override;

      void apply(DataTypeVisitor& visitor) const override;

    protected:
      bool is_equal(const DataType& right) const override;

    private:
      bool m_is_signed;
      int m_size;
  };

  //! 8-bit signed integer.
  static inline auto tiny_int = IntegerDataType(true, 1);

  //! 8-bit unsigned integer.
  static inline auto tiny_uint = IntegerDataType(false, 1);

  //! 16-bit signed integer.
  static inline auto small_int = IntegerDataType(true, 2);

  //! 16-bit unsigned integer.
  static inline auto small_uint = IntegerDataType(false, 2);

  //! 32-bit signed integer.
  static inline auto integer = IntegerDataType(true, 4);

  //! 32-bit unsigned integer.
  static inline auto uinteger = IntegerDataType(false, 4);

  //! 64-bit signed integer.
  static inline auto big_int = IntegerDataType(true, 8);

  //! 64-bit unsigned integer.
  static inline auto big_uint = IntegerDataType(false, 8);

  inline IntegerDataType::IntegerDataType(bool is_signed, int size)
      : m_is_signed(is_signed),
        m_size(size) {}

  inline bool IntegerDataType::is_signed() const {
    return m_is_signed;
  }

  inline int IntegerDataType::get_size() const {
    return m_size;
  }

  inline std::unique_ptr<DataType> IntegerDataType::clone() const {
    return std::make_unique<IntegerDataType>(*this);
  }

  inline void IntegerDataType::apply(DataTypeVisitor& visitor) const {
    visitor.visit(*this);
  }

  inline void DataTypeVisitor::visit(const IntegerDataType& node) {
    visit(static_cast<const DataType&>(node));
  }

  inline bool IntegerDataType::is_equal(const DataType& right) const {
    auto& right_int = static_cast<const IntegerDataType&>(right);
    return m_is_signed == right_int.is_signed() &&
      m_size == right_int.get_size();
  }
}

#endif
