#pragma once

namespace quip {
  struct Modifiers {
    bool control : 1;
    bool shift : 1;
    bool option : 1;
    bool command : 1;
    
    Modifiers();
    
    void clear();
  };
}
