#pragma once
#include <tuple>

namespace mvs {

template <typename T, typename V> struct value_patch {
  V T::*m;
  V v;
};

template <typename T, typename V> value_patch(V T::*, V) -> value_patch<T, V>;

template <typename T, typename... Args> struct struct_patch {
  std::tuple<value_patch<T, Args>...> impl;
};

template <typename T, typename... Args>
struct_patch(std::tuple<value_patch<T, Args>...>) -> struct_patch<T, Args...>;

template <typename T, typename V> T update(T t, value_patch<T, V> vl) {
  auto res{t};
  res.*vl.m = vl.v;
  return res;
}

template <typename T, typename V, typename... Args>
T update(T t, value_patch<T, V> df, Args... args) {
  return update(update(t, df), args...);
}

template <typename T, typename... Args>
T update(T t, struct_patch<T, Args...> d) {
  return update(t, d.impl);
}

template <typename T, typename... Args>
T update(T t, std::tuple<value_patch<T, Args>...> d) {
  auto res{t};

  std::apply([&res](auto... df) { res = update(res, df...); }, d);

  return res;
}

} // namespace mvs