#pragma once

namespace quip {
  struct Color {
    float r;
    float g;
    float b;
    float a;
    
    Color ();
    Color (float r, float g, float b);
    Color (float r, float g, float b, float a);
  };
}