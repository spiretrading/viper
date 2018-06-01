#ifndef VIPER_UTILITIES_HPP
#define VIPER_UTILITIES_HPP
#include <utility>

namespace viper {
namespace details {
  template<bool C>
  struct move_if_s {};

  template<>
  struct move_if_s<true> {
    template<typename T1, typename T2>
    auto operator ()(T1&& a, T2&& b) const -> decltype(auto) {
      return std::forward<T1>(a);
    }
  };

  template<>
  struct move_if_s<false> {
    template<typename T1, typename T2>
    auto operator ()(T1&& a, T2&& b) const -> decltype(auto)  {
      return std::forward<T2>(b);
    }
  };
}

  //! Moves one of two values depending on a compile-time condition.
  /*!
    \tparam C The compile time condition used to determine which value to move.
    \param a The first value to potentially move.
    \param b The second value to potentially move.
  */
  template<bool C, typename T1, typename T2>
  decltype(auto) move_if(T1&& a, T2&& b) {
    return details::move_if_s<C>()(std::forward<T1>(a), std::forward<T2>(b));
  }

  //! Moves a value depending on a series of compile-time conditions.
  /*!
    \tparam C1 Determines whether the first value should be moved.
    \tparam C2 Determines whether the second value should be moved.
    \param a The first value to potentially move.
    \param b The second value to potentially move.
    \param c The third value to potentially move.
  */
  template<bool C1, bool C2, typename T1, typename T2, typename T3>
  decltype(auto) move_if(T1&& a, T2&& b, T3&& c) {
    return move_if<C1>(std::forward<T1>(a), move_if<C2>(std::forward<T2>(b),
      std::forward<T3>(c)));
  }

  //! Moves a value depending on a series of compile-time conditions.
  /*!
    \tparam C1 Determines whether the first value should be moved.
    \tparam C2 Determines whether the second value should be moved.
    \tparam C3 Determines whether the third value should be moved.
    \param a The first value to potentially move.
    \param b The second value to potentially move.
    \param c The third value to potentially move.
    \param d The fourth value to potentially move.
  */
  template<bool C1, bool C2, bool C3, typename T1, typename T2, typename T3,
    typename T4>
  decltype(auto) move_if(T1&& a, T2&& b, T3&& c, T4&& d) {
    return move_if<C1>(std::forward<T1>(a), move_if<C2>(std::forward<T2>(b),
      move_if<C3>(std::forward<T3>(c), std::forward<T4>(d))));
  }

  //! Escapes special/reserved characters in an SQL string.
  /*!
    \param source The string to escape.
  */
  inline void escape(std::string_view source, std::string& destination) {
    destination += '\"';
    for(auto c : source) {
      if(c == '\0') {
        destination += "\\0";
      } else if(c == '\'') {
        destination += "\\'";
      } else if(c == '\"') {
        destination += "\\\"";
      } else if(c == '\x08') {
        destination += "\\b";
      } else if(c == '\n') {
        destination += "\\n";
      } else if(c == '\r') {
        destination += "\\r";
      } else if(c == '\t') {
        destination += "\\t";
      } else if(c == '\x1A') {
        destination += "\\n";
      } else if(c == '\\') {
        destination += "\\\\";
      } else {
        destination += c;
      }
    }
    destination += '\"';
  }
}

#endif
