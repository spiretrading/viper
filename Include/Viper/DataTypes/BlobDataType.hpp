#ifndef VIPER_BLOB_DATA_TYPE_HPP
#define VIPER_BLOB_DATA_TYPE_HPP
#include <cstdint>
#include "Viper/DataTypes/DataType.hpp"

namespace Viper {

  //! Represents an SQL blob.
  struct Blob {

    //! A pointer to the first byte in the blob.
    std::byte* m_data;

    //! The number of bytes in the blob.
    std::size_t m_size;
  };

  /** Represents an SQL variable length binary object up to a maximum size. */
  class BlobDataType final : public DataType {
    public:

      //! Constructs a blob with no specified max size.
      BlobDataType();

      //! Constructs a blob.
      /*!
        \param max_size The maximum size in bytes.
      */
      BlobDataType(int max_size);

      //! Returns the maximum size (in bytes) of the value.
      int get_max_size() const;

      std::unique_ptr<DataType> clone() const override;

      void apply(DataTypeVisitor& visitor) const override;

    protected:
      bool is_equal(const DataType& right) const override;

    private:
      int m_max_size;
  };

  //! Makes a blob with an unspecified max size.
  inline auto blob() {
    return BlobDataType();
  }

  //! Makes a blob with a maximum size.
  /*!
    \param max_size The maximum size in bytes.
  */
  inline auto blob(int max_size) {
    return BlobDataType(max_size);
  }

  inline BlobDataType::BlobDataType()
      : BlobDataType(-1) {}

  inline BlobDataType::BlobDataType(int max_size)
      : m_max_size(max_size) {}

  inline int BlobDataType::get_max_size() const {
    return m_max_size;
  }

  inline std::unique_ptr<DataType> BlobDataType::clone() const {
    return std::make_unique<BlobDataType>(*this);
  }

  inline void BlobDataType::apply(DataTypeVisitor& visitor) const {
    visitor.visit(*this);
  }

  inline void DataTypeVisitor::visit(const BlobDataType& node) {
    visit(static_cast<const DataType&>(node));
  }

  inline bool BlobDataType::is_equal(const DataType& right) const {
    auto& right_type = static_cast<const BlobDataType&>(right);
    return m_max_size == right_type.get_max_size();
  }
}

#endif
