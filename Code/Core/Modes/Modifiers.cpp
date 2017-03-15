#include "Modifiers.hpp"

namespace quip {
  Modifiers::Modifiers()
  : control(false)
  , shift(false)
  , option(false) {
  }
  
  void Modifiers::clear() {
    control = false;
    shift = false;
    option = false;
  }
}
