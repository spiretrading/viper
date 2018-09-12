#ifndef VIPER_MYSQL_DATA_TYPE_NAME_HPP
#define VIPER_MYSQL_DATA_TYPE_NAME_HPP
#include <string>
#include "Viper/DataTypes/DataTypeVisitor.hpp"
#include "Viper/DataTypes/DataTypes.hpp"

namespace Viper::MySql {

  //! Returns the MySQL compatible name of an SQL data type.
  inline std::string get_name(const DataType& t) {
    struct Visitor final : DataTypeVisitor {
      std::string m_result;

      void visit(const BlobDataType& type) override {
        if(type.get_max_size() < 0) {
          m_result = "BLOB";
        } else if(type.get_max_size() <= 255) {
          m_result = "TINYBLOB";
        } else if(type.get_max_size() <= 65535) {
          m_result = "BLOB";
        } else if(type.get_max_size() <= 16777215) {
          m_result = "MEDIUMBLOB";
        } else if(type.get_max_size() <= 4294967295) {
          m_result = "LONGBLOB";
        }
      }

      void visit(const FloatDataType& type) override {
        if(type.get_size() == 4) {
          m_result = "FLOAT";
        } else {
          m_result = "DOUBLE";
        }
      }

      void visit(const IntegerDataType& type) override {
        if(type.get_size() == 1) {
          m_result = "TINYINT";
        } else if(type.get_size() == 2) {
          m_result = "SMALLINT";
        } else if(type.get_size() == 3) {
          m_result = "MEDIUMINT";
        } else if(type.get_size() == 4) {
          m_result = "INT";
        } else {
          m_result = "BIGINT";
        }
        if(!type.is_signed()) {
          m_result += " UNSIGNED";
        }
      }

      void visit(const VarCharDataType& type) override {
        if(type.get_max_size() < 0) {
          m_result = "VARCHAR(64000) BINARY";
        } else {
          m_result = "VARCHAR(" + std::to_string(type.get_max_size()) +
            ") BINARY";
        }
      }
    };
    Visitor v;
    t.apply(v);
    return v.m_result;
  }
}

#endif
