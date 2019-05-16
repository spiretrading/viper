#ifndef VIPER_TEXT_DATA_TYPE_HPP
#define VIPER_TEXT_DATA_TYPE_HPP
#include "Viper/DataTypes/DataType.hpp"

namespace Viper {

  /** Represents an SQL text value. */
  class TextDataType final : public DataType {
    public:
      std::unique_ptr<DataType> clone() const override;

      void apply(DataTypeVisitor& visitor) const override;

    protected:
      bool is_equal(const DataType& right) const override;
  };

  //! Text data type.
  static inline auto text = TextDataType();

  inline std::unique_ptr<DataType> TextDataType::clone() const {
    return std::make_unique<TextDataType>(*this);
  }

  inline void TextDataType::apply(DataTypeVisitor& visitor) const {
    visitor.visit(*this);
  }

  inline void DataTypeVisitor::visit(const TextDataType& node) {
    visit(static_cast<const DataType&>(node));
  }

  inline bool TextDataType::is_equal(const DataType& right) const {
    return true;
  }
}

#endif
