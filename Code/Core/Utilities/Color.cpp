#include "Color.hpp"

namespace quip {
  Color::Color()
  : Color(1.0f, 1.0f, 1.0f, 1.0f) {
  }
  
  Color::Color(float r, float g, float b)
  : Color(r, g, b, 1.0f) {
  }
  
  Color::Color(float r, float g, float b, float a)
  : m_r(r)
  , m_g(g)
  , m_b(b)
  , m_a(a) {
  }
  
  float Color::r() const noexcept {
    return m_r;
  }
  
  float Color::g() const noexcept {
    return m_g;
  }
  
  float Color::b() const noexcept {
    return m_b;
  }
  
  float Color::a() const noexcept {
    return m_a;
  }
  
  Color Color::red() {
    return Color(1.0f, 0.0f, 0.0f, 1.0f);
  }
  
  Color Color::green() {
    return Color(0.0f, 1.0f, 0.0f, 1.0f);
  }
  
  Color Color::blue() {
    return Color(0.0f, 0.0f, 1.0f, 1.0f);
  }
  
  Color Color::transparent() {
    return Color(0.0f, 0.0f, 0.0f, 0.0f);
  }
  
  Color Color::black() {
    return Color(0.0f, 0.0f, 0.0f, 1.0f);
  }
  
  Color Color::white() {
    return Color(1.0f, 1.0f, 1.0f, 1.0f);
  }
}
