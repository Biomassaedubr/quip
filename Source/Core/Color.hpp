#pragma once

namespace quip {
  // A four-component floating point color.
  struct Color {
    Color ();
    Color (float red, float green, float blue);
    Color (float red, float green, float blue, float alpha);
    
    float red () const noexcept;
    float green () const noexcept;
    float blue () const noexcept;
    float alpha () const noexcept;
    
    static Color White;
    static Color Black;
    static Color Red;
    static Color Green;
    static Color Blue;
    
  private:
    float m_red;
    float m_green;
    float m_blue;
    float m_alpha;
  };
}