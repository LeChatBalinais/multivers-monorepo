#pragma once

#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <boost/mp11.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/variant2/variant.hpp>
#include <map>

using namespace boost::describe;

namespace mvs {

template <class T, class Md = describe_members<T, mod_any_access>,
          class En = std::enable_if_t<!std::is_union<T>::value>>
auto diff(T const &t1, T const &t2) {

  std::map<char const *, typename T::m_val_type> r;
  if (t1 == t2)
    return r;

  boost::mp11::mp_for_each<Md>([&](auto D) {
    if (t1.*D.pointer != t2.*D.pointer)
      r[D.name] = t2.*D.pointer;
  });

  return r;
}

template <class T, class Md = describe_members<T, mod_any_access>>
auto patch(T t, const std::map<char const *, typename T::m_val_type> &r) {

  auto res{t};

  boost::mp11::mp_for_each<Md>([&](auto D) {
    auto it{r.find(D.name)};
    if (it == r.end())
      return;
    res.*D.pointer = std::get<BOOST_TYPEOF(res.*D.pointer)>(it->second);
  });

  return res;
}

} // namespace mvs
