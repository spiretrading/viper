#ifndef VIPER_FLOAT_DATA_TYPE_HPP
#define VIPER_FLOAT_DATA_TYPE_HPP
#include "viper/data_types/data_type.hpp"

namespace viper {

  /** Represents an SQL floating point data type. */
  class float_data_type final : public data_type {
    public:

      //! Constructs an SQL float.
      /*!
        \param size The size (in bytes) of the float.
      */
      float_data_type(int size);

      //! Returns the size (in bytes) of the float.
      int get_size() const;

      std::unique_ptr<data_type> clone() const override;

      void apply(data_type_visitor& visitor) const override;

    protected:
      bool is_equal(const data_type& right) const override;

    private:
      int m_size;
  };

  //! 32-bit float.
  static inline auto f32 = float_data_type(4);

  //! 64-bit float.
  static inline auto f64 = float_data_type(8);

  inline float_data_type::float_data_type(int size)
      : m_size(size) {}

  inline int float_data_type::get_size() const {
    return m_size;
  }

  inline std::unique_ptr<data_type> float_data_type::clone() const {
    return std::make_unique<float_data_type>(*this);
  }

  inline void float_data_type::apply(data_type_visitor& visitor) const {
    visitor.visit(*this);
  }

  inline void data_type_visitor::visit(const float_data_type& node) {
    visit(static_cast<const data_type&>(node));
  }

  inline bool float_data_type::is_equal(const data_type& right) const {
    const auto& right_type = static_cast<const float_data_type&>(right);
    return m_size == right_type.get_size();
  }
}

#endif
