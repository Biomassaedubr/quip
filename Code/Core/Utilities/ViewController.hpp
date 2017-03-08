#pragma once

#include "Location.hpp"
#include "Signal.hpp"

namespace quip {
  struct ViewController {
    Signal<void(Location)> scrollToLocation;
    Signal<void(Location)> scrollLocationIntoView;
  };
}