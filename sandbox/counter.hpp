#pragma once

#include <immer/array.hpp>
#include <lager/extra/struct.hpp>
#include <lager/util.hpp>
#include <variant>

namespace counter {
struct model {
  immer::array<int> a;
};

struct increment_action {};
struct decrement_action {};
struct reset_action {
  immer::array<int> new_value = {};
};

using action = std::variant<increment_action, decrement_action, reset_action>;

inline model update(model c, action action) {
  return std::visit(lager::visitor{
                        [&](increment_action) { return model{c.a.push_back(1) }; },
                        [&](decrement_action) { return model{c.a.take(c.a.size()-1)}; },
                        [&](reset_action a) { return model{a.new_value}; },
                    },
                    action);
}

} // namespace coutner

LAGER_STRUCT(counter, model, a);
LAGER_STRUCT(counter, increment_action);
LAGER_STRUCT(counter, decrement_action);
LAGER_STRUCT(counter, reset_action, new_value);
