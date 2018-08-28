#ifndef VIPER_INDEX_HPP
#define VIPER_INDEX_HPP
#include <string>
#include <vector>

namespace Viper {

  //! Represents an SQL index.
  struct Index {

    //! The name of the index.
    std::string m_name;

    //! The names of the columns belonging to the index.
    std::vector<std::string> m_columns;

    //! Whether the index is unique.
    bool m_is_unique = false;

    //! Whether the index is a primary key.
    bool m_is_primary = false;
  };
}

#endif
