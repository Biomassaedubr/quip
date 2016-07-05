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
  
  Color::Color (float red, float green, float blue)
  : Color(red, green, blue, 1.0f) {
  }
  
  Color::Color (float red, float green, float blue, float alpha)
  : m_red(red)
  , m_green(green)
  , m_blue(blue)
  , m_alpha(alpha) {
  }
  
  float Color::red () const noexcept {
    return m_red;
  }
  
  float Color::green () const noexcept {
    return m_green;
  }
  
  float Color::blue () const noexcept {
    return m_blue;
  }
  
  float Color::alpha () const noexcept {
    return m_alpha;
  }
}