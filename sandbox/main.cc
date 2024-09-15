
#include "counter.hpp"
#include <iostream>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>

void draw(counter::model curr) {
  std::cout << "current value: " << curr.a.size() << '\n';
}

std::optional<counter::action> intent(char event) {
  switch (event) {
  case '+':
    return counter::increment_action{};
  case '-':
    return counter::decrement_action{};
  case '.':
    return counter::reset_action{};
  default:
    return std::nullopt;
  }
}

int main() {

  auto store = lager::make_store<counter::action>(
      counter::model{}, lager::with_manual_event_loop{});
  watch(store, draw);

  auto event = char{};
  while (std::cin >> event) {
    if (auto act = intent(event))
      store.dispatch(*act);
  }
}
