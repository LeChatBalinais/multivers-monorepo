#include "sandbox-describe/nested-struct-serialization.hh"
#include "sandbox-describe/json-serialization-utilities.hh"
#include "sandbox-describe/json-utils.hh"
#include "sandbox-describe/nested-struct-refl.hh"
#include "sandbox-describe/nested-struct.hh"
#include "sandbox-describe/type_traits_schm.hh"
#include <boost/json.hpp>
#include <optional>
#include <variant>
#include <vector>

namespace schema {

template <class T,
          class D1 = boost::describe::describe_members<
              T, boost::describe::mod_public | boost::describe::mod_protected |
                     boost::describe::mod_inherited>,
          class D2 = boost::describe::describe_members<
              T, boost::describe::mod_private>,
          class En = std::enable_if_t<boost::mp11::mp_empty<D2>::value &&
                                      !std::is_union<T>::value>>

void tag_invoke(boost::json::value_from_tag const &vt, boost::json::value &v,
                T const &t) {
  dscrb_json::tag_invoke<T, D1, D2, En>(vt, v, t);
}

template <class T,
          class D1 = boost::describe::describe_members<
              T, boost::describe::mod_public | boost::describe::mod_protected |
                     boost::describe::mod_inherited>,
          class D2 = boost::describe::describe_members<
              T, boost::describe::mod_private>,
          class En = std::enable_if_t<boost::mp11::mp_empty<D2>::value &&
                                      !std::is_union<T>::value>>

T tag_invoke(boost::json::value_to_tag<T> const &vt,
             boost::json::value const &v) {
  return dscrb_json::tag_invoke<
      T, D1, D2, En,
      dscrb_json::value_to<elem_tags, description_variable_part_tags>>(vt, v);
}

MaybeElement parse(std::istream &is) {

  auto jv{tst::parse_stream(is)};

  if (!jv.if_object())
    return {};
  try {
    MaybeElement el;

    dscrb_json::value_to<elem_tags, description_variable_part_tags>::set(jv,
                                                                         el);
    return el;
  } catch (...) {
    return {};
  }
}

template <typename Var>
void marshallVariant_helper(std::ostream &os, const Var &value) {}

template <typename Var, typename T, typename... REST>
void marshallVariant_helper(std::ostream &os, const Var &value) {
  if (std::holds_alternative<T>(value)) {
    os << dscrb_json::value_from::get(std::get<T>(value));
  } else {
    marshallVariant_helper<Var, REST...>(os, value);
  }
}

template <typename... T>
void marshallVariant(std::ostream &os, const std::variant<T...> &el) {
  marshallVariant_helper<std::variant<T...>, T...>(os, el);
}

void marshall(std::ostream &os, const Element &el) { marshallVariant(os, el); }

} // namespace schema