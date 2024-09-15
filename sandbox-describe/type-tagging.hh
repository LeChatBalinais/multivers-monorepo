#pragma once

#include "type_traits_schm.hh"
#include <optional>
#include <variant>

namespace schm {

template <typename B, typename E, std::optional<E> B::*member> struct tagging {
  template <E instance, typename T> struct tagged_type {
    using object_class = T;
    static const E enumValue{instance};
  };

  template <E e, typename T/*,
            typename std::enable_if<std::is_base_of<B, T>::value>::type * =
                nullptr*/>
  using tag = tagged_type<e, T>;

  template <typename... T> struct tag_mapping {
    template <E e, typename... M> struct type_helper {};

    template <E e, typename TT> struct type_helper<e, TT> {
      using t = typename std::conditional<e == TT::enumValue,
                                          typename TT::object_class, B>::type;
    };

    template <E e, typename TT1, typename... TT>
    struct type_helper<e, TT1, TT...> {
      using t =
          typename std::conditional<e == TT1::enumValue,
                                    typename TT1::object_class,
                                    typename type_helper<e, TT...>::t>::type;
    };

    template <E e, typename std::enable_if<is_one_of_values<
                       E, e, T::enumValue...>::value>::type * = nullptr>
    using tp = typename type_helper<e, T...>::t;

    using Variant = std::variant<typename T::object_class...>;
    using MaybeVariant =
        std::variant<std::monostate, typename T::object_class...>;

    template <typename C> constexpr static E enumValue_helper() { return {}; }

    template <typename C, typename T1, typename... TT>
    constexpr static E enumValue_helper() {
      if constexpr (std::is_same_v<C, typename T1::object_class>) {
        return T1::enumValue;
      }
      return enumValue_helper<C, TT...>();
    }

    template <typename C,
              typename std::enable_if<is_one_of_types<
                  C, typename T::object_class...>::value>::type * = nullptr>
    constexpr static E enumValue() {
      return enumValue_helper<C, T...>();
    }

    using EnumType = E;
    using BaseType = B;
    constexpr static std::optional<E> B::*tagMember{member};

    template <class ST>
    struct type_is_tagged
        : schm::is_one_of_types<ST, typename T::object_class...>::type {};
  };
};
} // namespace schm