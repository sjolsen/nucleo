#ifndef REGISTERS_REGISTER_HH
#define REGISTERS_REGISTER_HH

#include <string.h>

template <typename R>
struct Register
{
  using register_type = typename R::register_type;

  register_type raw;

  operator R() const volatile& {
    R result;
    register_type raw_value = raw;
    memcpy(&result, &raw_value, sizeof(raw_value));
    return result;
  }

  void operator = (const R& value) volatile& {
    register_type raw_value;
    memcpy(&raw_value, &value, sizeof(raw_value));
    raw = raw_value;
  }
};

#endif
