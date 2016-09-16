#pragma once

namespace quip {
  // A four-component floating point color.
  struct Color {
    Color ();
    Color (float r, float g, float b);
    Color (float r, float g, float b, float a);
    
    float r () const noexcept;
    float g () const noexcept;
    float b () const noexcept;
    float a () const noexcept;
    
    static Color red();
    static Color green();
    static Color blue();
    static Color transparent();
    static Color black();
    static Color white();
    
  private:
    float m_r;
    float m_g;
    float m_b;
    float m_a;
  };
}