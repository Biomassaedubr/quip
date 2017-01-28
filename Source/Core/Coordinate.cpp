#include "Coordinate.hpp"

namespace quip {
  Coordinate::Coordinate()
  : Coordinate(0.0f, 0.0f) {
  }
  
  Coordinate::Coordinate(float x, float y)
  : x(x)
  , y(y) {
  }
}
