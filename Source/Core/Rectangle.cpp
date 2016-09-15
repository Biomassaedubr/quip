#include "Rectangle.hpp"

namespace quip {
  Rectangle::Rectangle () noexcept
  : Rectangle(0.0f, 0.0f, 0.0f, 0.0f) {
  }
  
  Rectangle::Rectangle (float x, float y, float width, float height) noexcept
  : m_x(x)
  , m_y(y)
  , m_width(width)
  , m_height(height) {
  }
  
  Rectangle::Rectangle (const Rectangle & other) noexcept
  : m_x(other.m_x)
  , m_y(other.m_y)
  , m_width(other.m_width)
  , m_height(other.m_height) {
  }
  
  Rectangle::Rectangle (Rectangle && other) noexcept
  : m_x(other.m_x)
  , m_y(other.m_y)
  , m_width(other.m_width)
  , m_height(other.m_height) {
  }
  
  Rectangle & Rectangle::operator= (const Rectangle & other) noexcept {
    m_x = other.m_x;
    m_y = other.m_y;
    m_width = other.m_width;
    m_height = other.m_height;
    return *this;
  }
  
  Rectangle & Rectangle::operator= (Rectangle && other) noexcept {
    m_x = other.m_x;
    m_y = other.m_y;
    m_width = other.m_width;
    m_height = other.m_height;
    return *this;
  }
  
  float Rectangle::x () const noexcept {
    return m_x;
  }
  
  float Rectangle::y () const noexcept {
    return m_y;
  }
  
  float Rectangle::width () const noexcept {
    return m_width;
  }
  
  float Rectangle::height () const noexcept {
    return m_height;
  }
}