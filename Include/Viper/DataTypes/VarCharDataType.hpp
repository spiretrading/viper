#ifndef VIPER_VARCHAR_DATA_TYPE_HPP
#define VIPER_VARCHAR_DATA_TYPE_HPP
#include "viper/data_types/data_type.hpp"

namespace Viper {

  /** Represents an SQL variable length string up to a maximum size. */
  class VarCharDataType final : public DataType {
    public:

      //! Constructs a varchar with no specified max size.
      VarCharDataType();

      //! Constructs a varchar.
      /*!
        \param max_size The maximum size in bytes.
      */
      VarCharDataType(int max_size);

      //! Returns the maximum size (in bytes) of the value.
      int get_max_size() const;

      std::unique_ptr<DataType> clone() const override;

      void apply(DataTypeVisitor& visitor) const override;

    protected:
      bool is_equal(const DataType& right) const override;

    private:
      int m_max_size;
  };

  //! Makes a varchar with an unspecified max size.
  inline auto varchar() {
    return VarCharDataType();
  }

  //! Makes a varchar with a maximum size.
  /*!
    \param max_size The maximum size in bytes.
  */
  inline auto varchar(int max_size) {
    return VarCharDataType(max_size);
  }

  inline VarCharDataType::VarCharDataType()
      : VarCharDataType(-1) {}

  inline VarCharDataType::VarCharDataType(int max_size)
      : m_max_size(max_size) {}

  inline int VarCharDataType::get_max_size() const {
    return m_max_size;
  }

  inline std::unique_ptr<DataType> VarCharDataType::clone() const {
    return std::make_unique<VarCharDataType>(*this);
  }

  inline void VarCharDataType::apply(DataTypeVisitor& visitor) const {
    visitor.visit(*this);
  }

  inline void DataTypeVisitor::visit(const VarCharDataType& node) {
    visit(static_cast<const DataType&>(node));
  }

  inline bool VarCharDataType::is_equal(const DataType& right) const {
    auto& right_type = static_cast<const VarCharDataType&>(right);
    return m_max_size == right_type.get_max_size();
  }
}

#endif
