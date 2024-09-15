#pragma once

#include "sandbox-describe/type_traits_schm.hh"
#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <boost/mp11.hpp>
#include <optional>

namespace dscrb_json {

struct value_from {
  template <typename T> static boost::json::value get(const T &t) {
    if constexpr (std::is_enum<T>::value) {

      return boost::json::value_from(boost::describe::enum_to_string(t, ""));

    } else if constexpr (std::is_same<T, double>::value) {

      if (std::isfinite(t))
        return boost::json::value_from(t);

      return boost::json::value_from(std::to_string(t));
    } else {

      return boost::json::value_from(t);
    }
  }
  template <typename T>
  static boost::json::value get(const std::optional<T> &t) {
    if (!t)
      return {};

    return get(*t);
  }

private:
  template <typename V>
  static boost::json::value value_from_variant(const V &value) {
    return {};
  }
  template <typename V, typename T, class... REST>
  static boost::json::value value_from_variant(const V &value) {

    if (std::holds_alternative<T>(value)) {

      return get(std::get<T>(value));
    } else {

      return value_from_variant<V, REST...>(value);
    }
  }

public:
  template <class... T>
  static boost::json::value get(const std::variant<T...> &value) {
    return value_from_variant<std::variant<T...>, T...>(value);
  }

  template <class T>
  static boost::json::value get(const std::vector<T> &value) {

    boost::json::array ar;

    for (auto &e : value) {
      ar.push_back(get(e));
    }
    return ar;
  }
};

template <class T, class D1, class D2, class En, class VF = value_from>
void tag_invoke(boost::json::value_from_tag const &, boost::json::value &v,
                T const &t) {
  auto &obj = v.emplace_object();

  boost::mp11::mp_for_each<D1>([&](auto D) {
    boost::json::value jv{VF::get(t.*D.pointer)};
    if (!jv.is_null())
      obj[D.name] = jv;
  });
}

template <class Labeling, class... T> struct is_labeling_suitable_for {};

template <class Labeling, class T>
struct is_labeling_suitable_for<Labeling, T>
    : Labeling::template type_is_tagged<T>::type {};

template <class Labeling, class T1, class... T>
struct is_labeling_suitable_for<Labeling, T1, T...>
    : std::conditional<
          (std::is_base_of<typename Labeling::BaseType, T1>::value &&
           Labeling::template type_is_tagged<T1>::value),
          is_labeling_suitable_for<Labeling, T...>, std::false_type>::type {};

template <class... Labelings> struct value_to {

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

  template <class T> static void set(const boost::json::value &jv, T &value) {

    if constexpr (std::is_enum<T>::value) {
      auto mbString{jv.if_string()};
      boost::describe::enum_from_string(mbString->c_str(), value);
    } else if constexpr (std::is_same<T, double>::value) {
      if (auto doublePtr{jv.if_double()}) {
        value = *doublePtr;
      } else if (auto stringPtr{jv.if_string()}) {
        value = std::stod(stringPtr->c_str());
      } else if (auto int64Ptr{jv.if_int64()}) {
        value = *int64Ptr;
      }
    } else
      value = boost::json::value_to<T>(jv);
  }

  template <class T>
  static void set(const boost::json::value &jv, std::optional<T> &value) {
    T v;
    set(jv, v);
    value = std::make_optional<T>(v);
  }

  template <class T>
  static void set(const boost::json::value &jv, std::vector<T> &value) {
    auto ja{jv.if_array()};

    if (!ja)
      return;

    for (auto &jae : *ja) {
      T v;
      set(jae, v);
      value.push_back(v);
    }
  }

  template <
      class... T,
      typename Lbl =
          typename type_list<T...>::template labeling_helper<Labelings...>::t,
      typename std::enable_if<!std::is_same<std::false_type, Lbl>::value>::type
          * = nullptr>
  static void set(const boost::json::value &jv,
                  std::variant<std::monostate, T...> &value) {
    typename Lbl::BaseType e;
    e = boost::json::value_to<typename Lbl::BaseType>(jv);

    boost::mp11::mp_for_each<boost::describe::describe_enumerators<
        typename Lbl::EnumType>>([&](auto D) {
      if (*(e.*(Lbl::tagMember)) == D.value) {

        if constexpr (schm::is_one_of_types<typename Lbl::template tp<D.value>,
                                            T...>::value) {
          typename Lbl::template tp<D.value> ce;
          set(jv, ce);
          value = ce;
        }
      }
    });
  }

  template <
      class... T,
      typename Lbl =
          typename type_list<T...>::template labeling_helper<Labelings...>::t,
      typename std::enable_if<!std::is_same<std::false_type, Lbl>::value>::type
          * = nullptr>
  static void set(const boost::json::value &jv, std::variant<T...> &value) {
    typename Lbl::BaseType e;
    e = boost::json::value_to<typename Lbl::BaseType>(jv);

    boost::mp11::mp_for_each<boost::describe::describe_enumerators<
        typename Lbl::EnumType>>([&](auto D) {
      if (*(e.*(Lbl::tagMember)) == D.value) {

        if constexpr (schm::is_one_of_types<typename Lbl::template tp<D.value>,
                                            T...>::value) {
          typename Lbl::template tp<D.value> ce;
          set(jv, ce);
          value = ce;
        }
      }
    });
  }
};

template <class T, class D1, class D2, class En, class VT>
T tag_invoke(boost::json::value_to_tag<T> const &,
             boost::json::value const &v) {
  auto const &obj = v.as_object();

  T t{};

  boost::mp11::mp_for_each<D1>([&](auto D) {
    auto jvPtr{obj.find(D.name)};
    if (jvPtr != obj.end())
      VT::set(jvPtr->value(), t.*D.pointer);
  });

  return t;
}

} // namespace dscrb_json