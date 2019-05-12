#ifndef VIPER_SQLITE3_DATA_TYPE_NAME_HPP
#define VIPER_SQLITE3_DATA_TYPE_NAME_HPP
#include <string>
#include "Viper/DataTypes/DataTypeVisitor.hpp"
#include "Viper/DataTypes/DataTypes.hpp"

namespace Viper::Sqlite3 {

  //! Returns the sqlite3 compatible name of an SQL data type.
  inline std::string get_name(const DataType& t) {
    struct Visitor final : DataTypeVisitor {
      std::string m_result;

      void visit(const BlobDataType& type) override {
        m_result = "BLOB";
      }

      void visit(const DateTimeDataType& type) override {
        m_result = "DATETIME";
      }

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
    Visitor v;
    t.apply(v);
    return v.m_result;
  }
}

#endif
