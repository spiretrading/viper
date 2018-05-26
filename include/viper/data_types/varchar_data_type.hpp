#ifndef VIPER_VARCHAR_DATA_TYPE_HPP
#define VIPER_VARCHAR_DATA_TYPE_HPP
#include "viper/data_types/data_type.hpp"

namespace viper {

  /** Represents an SQL variable length string up to a maximum size. */
  class varchar_data_type final : public data_type {
    public:

      //! Constructs a varchar with no specified max size.
      varchar_data_type();

      //! Constructs a varchar.
      /*!
        \param max_size The maximum size in bytes.
      */
      varchar_data_type(int max_size);

      //! Returns the maximum size (in bytes) of the value.
      int get_max_size() const;

      std::unique_ptr<data_type> clone() const override;

      void apply(data_type_visitor& visitor) const override;

    protected:
      bool is_equal(const data_type& right) const override;

    private:
      int m_max_size;
  };

  //! Makes a varchar with an unspecified max size.
  inline auto varchar() {
    return varchar_data_type();
  }

  //! Makes a varchar with a maximum size.
  /*!
    \param max_size The maximum size in bytes.
  */
  inline auto varchar(int max_size) {
    return varchar_data_type(max_size);
  }

  inline varchar_data_type::varchar_data_type()
      : varchar_data_type(-1) {}

  inline varchar_data_type::varchar_data_type(int max_size)
      : m_max_size(max_size) {}

  inline int varchar_data_type::get_max_size() const {
    return m_max_size;
  }

  inline std::unique_ptr<data_type> varchar_data_type::clone() const {
    return std::make_unique<varchar_data_type>(*this);
  }

  inline void varchar_data_type::apply(data_type_visitor& visitor) const {
    visitor.visit(*this);
  }

  inline void data_type_visitor::visit(const varchar_data_type& node) {
    visit(static_cast<const data_type&>(node));
  }

  inline bool varchar_data_type::is_equal(const data_type& right) const {
    const auto& right_type = static_cast<const varchar_data_type&>(right);
    return m_max_size == right_type.get_max_size();
  }
}

#endif
