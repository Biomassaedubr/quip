#include "Color.hpp"

namespace quip {
  Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);
  Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);
  Color Color::Red(1.0f, 0.0f, 0.0f, 1.0f);
  Color Color::Green(0.0f, 1.0f, 0.0f, 1.0f);
  Color Color::Blue(0.0f, 0.0f, 1.0f, 1.0f);
  
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