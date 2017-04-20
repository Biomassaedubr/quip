#pragma once

#include <functional>

#include "optional_lite.hpp"

namespace quip {
  template<typename T>
  using Optional = nonstd::optional_lite::optional<T>;
}
