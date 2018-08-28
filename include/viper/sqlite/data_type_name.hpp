#ifndef VIPER_SQLITE_DATA_TYPE_NAME_HPP
#define VIPER_SQLITE_DATA_TYPE_NAME_HPP
#include <string>
#include "viper/data_types/data_type_visitor.hpp"
#include "viper/data_types/data_types.hpp"

namespace Viper::sqlite3 {

  //! Returns the sqlite3 compatible name of an SQL data type.
  inline std::string get_name(const DataType& t) {
    struct visitor final : DataTypeVisitor {
      std::string m_result;

      void visit(const FloatDataType& type) override {
        m_result = "REAL";
      }

      void visit(const IntegerDataType& type) override {
        m_result = "INTEGER";
      }

      void visit(const VarCharDataType& type) override {
        m_result = "TEXT";
      }
    };
    visitor v;
    t.apply(v);
    return v.m_result;
  }
}

#endif
