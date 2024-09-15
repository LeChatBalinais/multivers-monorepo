#pragma once

#include <memory>
#include <utility>

template <typename T> class value_pst {
  struct shared_part {
    template <typename... Args>
    explicit shared_part(Args &&...args)
        : value_ptr{new T{std::forward<Args>(args)...}} {}

    std::unique_ptr<T> value_ptr;
  };

public:
  template <typename... Args>
  value_pst(Args &&...args)
      : sp{new shared_part{std::forward<Args>(args)...}} {}

  friend void swap(value_pst &a, value_pst &b) {
    using std::swap;
    swap(a.sp, b.sp);
  }

  value_pst(value_pst &&other) { swap(*this, other); }
  value_pst(const value_pst &other) : sp(other.sp) {}

protected:
  T &get() const { return *(sp->value_ptr); }

private:
  std::shared_ptr<shared_part> sp;
};

#define DECLARE_STRUCT(struct_name, value_name_1)                              \
  static_assert(std::is_class<struct_name>::value,                             \
                "DECLARE_STRUCT should only be used with class types");        \
  class struct_name##Transient : public value_pst<struct_name> {               \
  public:                                                                      \
    using value_pst<struct_name>::value_pst;                                   \
    const auto &get_##value_name_1() { return get().value_name_1; }            \
    template <typename T> void set_##value_name_1(T &&v) {                     \
      get().value_name_1 = v;                                                  \
    }                                                                          \
  };                                                                           \
  class struct_name##Persistent : public value_pst<struct_name> {              \
  public:                                                                      \
    using value_pst<struct_name>::value_pst;                                   \
    const auto &get_##value_name_1() { return get().value_name_1; }            \
  };
