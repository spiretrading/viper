#ifndef VIPER_INTEGER_DATA_TYPE_HPP
#define VIPER_INTEGER_DATA_TYPE_HPP
#include "viper/data_types/data_type.hpp"

namespace viper {

  /** Represents an SQL integer of some fixed byte size. */
  class integer_data_type final : public data_type {
    public:

      //! Constructs an SQL integer.
      /*!
        \param is_signed Whether the integer is signed.
        \param size The size (in bytes) of the integer.
      */
      integer_data_type(bool is_signed, int size);

      //! Returns true iff this is a signed integer.
      bool is_signed() const;

      //! Returns the size (in bytes) of the integer.
      int get_size() const;

      std::unique_ptr<data_type> clone() const override;

      void apply(data_type_visitor& visitor) const override;

    protected:
      bool is_equal(const data_type& right) const override;

    private:
      bool m_is_signed;
      int m_size;
  };

  //! 8-bit signed integer.
  static inline auto tiny_int = integer_data_type(true, 1);

  //! 8-bit unsigned integer.
  static inline auto tiny_uint = integer_data_type(false, 1);

  //! 16-bit signed integer.
  static inline auto small_int = integer_data_type(true, 2);

  //! 16-bit unsigned integer.
  static inline auto small_uint = integer_data_type(false, 2);

  //! 32-bit signed integer.
  static inline auto integer = integer_data_type(true, 4);

  //! 32-bit unsigned integer.
  static inline auto uinteger = integer_data_type(false, 4);

  //! 64-bit signed integer.
  static inline auto big_int = integer_data_type(true, 8);

  //! 64-bit unsigned integer.
  static inline auto big_uint = integer_data_type(false, 8);

  inline integer_data_type::integer_data_type(bool is_signed, int size)
      : m_is_signed(is_signed),
        m_size(size) {}

  inline bool integer_data_type::is_signed() const {
    return m_is_signed;
  }

  inline int integer_data_type::get_size() const {
    return m_size;
  }

  inline std::unique_ptr<data_type> integer_data_type::clone() const {
    return std::make_unique<integer_data_type>(*this);
  }

  inline void integer_data_type::apply(data_type_visitor& visitor) const {
    visitor.visit(*this);
  }

  inline void data_type_visitor::visit(const integer_data_type& node) {
    visit(static_cast<const data_type&>(node));
  }

  inline bool integer_data_type::is_equal(const data_type& right) const {
    const auto& right_int = static_cast<const integer_data_type&>(right);
    return m_is_signed == right_int.is_signed() &&
      m_size == right_int.get_size();
  }
}

#endif
