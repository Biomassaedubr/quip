#pragma once

namespace quip {
  // A floating-point rectangle represented by an and a size.
  struct Rectangle {
    Rectangle() noexcept;
    Rectangle(float x, float y, float width, float height) noexcept;
    Rectangle(const Rectangle& other) noexcept;
    Rectangle(Rectangle&& other) noexcept;
    
    Rectangle& operator=(const Rectangle& other) noexcept;
    Rectangle& operator=(Rectangle&& other) noexcept;
    
    float x() const noexcept;
    float y() const noexcept;
    float width() const noexcept;
    float height() const noexcept;
    
  private:
    float m_x;
    float m_y;
    float m_width;
    float m_height;
  };
}
