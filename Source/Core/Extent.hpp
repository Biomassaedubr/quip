#pragma once

namespace quip {
  // A floating-point size.
  struct Extent {
    Extent();
    Extent(float width, float height);
    
    float width() const;
    float height() const;
    
  private:
    float m_width;
    float m_height;
  };
}
