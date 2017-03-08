#include "Extent.hpp"

namespace quip {
  Extent::Extent()
  : Extent(0.0f, 0.0f) {
  }
  
  Extent::Extent(float width, float height)
  : m_width(width)
  , m_height(height) {
  }
  
  float Extent::width() const {
    return m_width;
  }
  
  float Extent::height() const {
    return m_height;
  }
}
