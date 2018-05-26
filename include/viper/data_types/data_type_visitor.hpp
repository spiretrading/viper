#ifndef VIPER_DATA_TYPE_VISITOR_HPP
#define VIPER_DATA_TYPE_VISITOR_HPP

namespace viper {
  class data_type;
  class integer_data_type;

  //! Implements the visitor pattern for SQL data types.
  class data_type_visitor {
    public:
      virtual ~data_type_visitor() = default;

      virtual void visit(const data_type& type);

      virtual void visit(const integer_data_type& type);

    protected:

      //! Constructs an SQL data type visitor.
      data_type_visitor() = default;
  };

  inline void data_type_visitor::visit(const data_type& type) {}
}

#endif
