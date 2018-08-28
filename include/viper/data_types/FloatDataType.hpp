#ifndef VIPER_FLOAT_DATA_TYPE_HPP
#define VIPER_FLOAT_DATA_TYPE_HPP
#include "viper/data_types/data_type.hpp"

namespace Viper {

  /** Represents an SQL floating point data type. */
  class FloatDataType final : public DataType {
    public:

      //! Constructs an SQL float.
      /*!
        \param size The size (in bytes) of the float.
      */
      FloatDataType(int size);

      //! Returns the size (in bytes) of the float.
      int get_size() const;

      std::unique_ptr<DataType> clone() const override;

      void apply(DataTypeVisitor& visitor) const override;

    protected:
      bool is_equal(const DataType& right) const override;

    private:
      int m_size;
  };

  //! 32-bit float.
  static inline auto f32 = FloatDataType(4);

  //! 64-bit float.
  static inline auto f64 = FloatDataType(8);

  inline FloatDataType::FloatDataType(int size)
      : m_size(size) {}

  inline int FloatDataType::get_size() const {
    return m_size;
  }

  inline std::unique_ptr<DataType> FloatDataType::clone() const {
    return std::make_unique<FloatDataType>(*this);
  }

  inline void FloatDataType::apply(DataTypeVisitor& visitor) const {
    visitor.visit(*this);
  }

  inline void DataTypeVisitor::visit(const FloatDataType& node) {
    visit(static_cast<const DataType&>(node));
  }

  inline bool FloatDataType::is_equal(const DataType& right) const {
    auto& right_type = static_cast<const FloatDataType&>(right);
    return m_size == right_type.get_size();
  }
}

#endif
