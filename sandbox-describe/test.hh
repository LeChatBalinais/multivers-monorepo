#pragma once

#include "sandbox-describe/type_traits_schm.hh"
#include <type_traits>
#include <variant>
#include <vector>
namespace test {

template <class Labeling, class... T> struct is_labeling_suitable_for {};

template <class Labeling, class T>
struct is_labeling_suitable_for<Labeling, T>
    : Labeling::template type_is_tagged<T>::type {};

template <class Labeling, class T1, class... T>
struct is_labeling_suitable_for<Labeling, T1, T...>
    : std::conditional<(std::is_base_of<typename Labeling::Base, T1>::value &&
                        Labeling::template type_is_tagged<T1>::value),
                       is_labeling_suitable_for<Labeling, T...>,
                       std::false_type>::type {};

template <class... Labelings> class Counting {

  template <typename... T> struct type_list {

    template <typename... L> struct labeling_helper {};

    template <typename L> struct labeling_helper<L> {
      using t =
          typename std::conditional<is_labeling_suitable_for<L, T...>::value, L,
                                    std::false_type>::type;
    };

    template <typename L1, typename... L> struct labeling_helper<L1, L...> {
      using t =
          typename std::conditional<is_labeling_suitable_for<L1, T...>::value,
                                    L1,
                                    typename labeling_helper<L...>::t>::type;
    };
  };

public:
  template <
      class... T,
      typename Lbl =
          typename type_list<T...>::template labeling_helper<Labelings...>::t,
      typename std::enable_if<!std::is_same<std::false_type, Lbl>::value>::type
          * = nullptr>
  static int count(const std::variant<T...> &o) {

    count(*o);
  }
};

} // namespace test