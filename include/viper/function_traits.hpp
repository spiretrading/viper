#ifndef VIPER_FUNCTION_TRAITS_HPP
#define VIPER_FUNCTION_TRAITS_HPP
#include <functional>
#include <utility>

namespace Viper {
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
  struct get_argument {};

  template<typename R, typename A1, typename A2>
  struct get_argument<std::function<R (A1, A2)>> {
    using type = std::decay_t<A2>;
  };

  template<typename F>
  using get_argument_t = typename get_argument<F>::type;

  template<typename F>
  auto make_function(F&& f) {
    return typename function_traits<F>::type(std::forward<F>(f));
  }
}

#endif
