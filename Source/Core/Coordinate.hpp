#pragma once

namespace quip {
  // A floating-point 2D coordinate.
  struct Coordinate {
    float x;
    float y;
    
    Coordinate();
    Coordinate(float x, float u);
  };
}
