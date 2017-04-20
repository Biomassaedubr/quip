#include "Modifiers.hpp"

namespace quip {
  Modifiers::Modifiers()
  : control(false)
  , option(false)
  , shift(false) {
  }
  
  void Modifiers::clear() {
    control = false;
    option = false;
    shift = false;
  }
}
