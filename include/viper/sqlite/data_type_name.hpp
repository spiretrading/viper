#ifndef VIPER_SQLITE_DATA_TYPE_NAME_HPP
#define VIPER_SQLITE_DATA_TYPE_NAME_HPP
#include <string>
#include "viper/data_types/data_type_visitor.hpp"
#include "viper/data_types/data_types.hpp"

namespace viper::sqlite3 {

  //! Returns the sqlite3 compatible name of an SQL data type.
  inline std::string get_name(const data_type& t) {
    struct visitor final : data_type_visitor {
      std::string m_result;

      void visit(const integer_data_type& type) override {
        m_result = "INTEGER";
      }

      void visit(const varchar_data_type& type) override {
        m_result = "TEXT";
      }
    };
    visitor v;
    t.apply(v);
    return v.m_result;
  }
}

#endif
