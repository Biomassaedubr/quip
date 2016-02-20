#include "Color.hpp"

namespace quip {
  Color::Color ()
  : Color(1.0f, 1.0f, 1.0f, 1.0f) {
  }
  
  Color::Color (float r, float g, float b)
  : Color(r, g, b, 1.0f) {
  }
  
  Color::Color (float r, float g, float b, float a)
  : r(r)
  , g(g)
  , b(b)
  , a(a) {
  }
}