#ifndef VIPER_FUNCTION_TRAITS_HPP
#define VIPER_FUNCTION_TRAITS_HPP
#include <functional>
#include <utility>

namespace viper {
  template<typename T>
  struct function_traits : function_traits<decltype(&T::operator())> {};

  template<typename R, typename... A>
  struct function_traits<R (A...)> {
    using type = std::function<R (A...)>;
  };

  template<typename R, typename... A>
  struct function_traits<R (*)(A...)> : function_traits<R (A...)> {};

  template<typename T, typename R, typename... A>
  struct function_traits<R (T::*)(A...)> : function_traits<R (*)(A...)> {};

  template<typename T, typename R, typename... A>
  struct function_traits<R (T::*)(A...) const> :
    function_traits<R (T::*)(A...)> {};

  template<typename F>
  auto make_function(F&& f) {
    return typename function_traits<F>::type(std::forward<F>(f));
  }
}

#endif
