#pragma once

#include <type_traits>
#include <variant>

namespace schm {

template <class T, class... U> struct is_one_of_types {};

template <class T, class U>
struct is_one_of_types<T, U> : std::is_same<T, U> {};

template <class T, class U1, class... U>
struct is_one_of_types<T, U1, U...>
    : std::conditional<std::is_same<T, U1>::value, std::true_type,
                       is_one_of_types<T, U...>>::type {};

template <class E, E... e> struct is_one_of_values {};

template <class E, E t, E u>
struct is_one_of_values<E, t, u>
    : std::conditional<t == u, std::true_type, std::false_type>::type {};

template <class E, E t, E u1, E... u>
struct is_one_of_values<E, t, u1, u...>
    : std::conditional<t == u1, std::true_type,
                       is_one_of_values<E, t, u...>>::type {};

template <class Base, class... Derived> struct is_base_of_each {};

template <class Base, class Derived>
struct is_base_of_each<Base, Derived>
    : std::conditional<std::is_base_of<Base, Derived>::value, std::true_type,
                       std::false_type>::type {};

template <class Base, class Derived1, class... Derived>
struct is_base_of_each<Base, Derived1, Derived...>
    : std::conditional<std::is_base_of<Base, Derived1>::value,
                       is_base_of_each<Base, Derived...>,
                       std::false_type>::type {};

template <class E, E u1> constexpr bool is_one_of_values_f(E t) {
  return t == u1;
}
template <class E, E u1, E... u> constexpr bool is_one_of_values_f(E t) {
  if (t == u1)
    return true;
  return is_one_of_values_f<u...>(t);
}
template <typename Var, typename MaybeVar>
Var fromMaybeVariant_helper(const MaybeVar &value) {
  return {};
}

template <typename Var, typename MaybeVar, typename T, typename... REST>
Var fromMaybeVariant_helper(const MaybeVar &value) {
  if (std::holds_alternative<T>(value)) {
    return Var(std::get<T>(value));
  } else {
    return fromMaybeVariant_helper<Var, MaybeVar, REST...>(value);
  }
}

template <typename... T>
std::variant<T...> fromMaybeVariant(std::variant<std::monostate, T...> v) {
  return fromMaybeVariant_helper<std::variant<T...>,
                                 std::variant<std::monostate, T...>, T...>(v);
}
template <typename... T>
bool holds_monostate(const std::variant<std::monostate, T...> v) {
  return std::holds_alternative<std::monostate>(v);
}
} // namespace schm