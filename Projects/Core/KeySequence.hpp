#pragma once

#include "Key.hpp"
#include "Modifiers.hpp"

#include <string>
#include <vector>

namespace quip {
  // A sequence of key presses.
  //
  // A KeySequence is mainly used to associate input with commands in a Mode subclass.
  struct KeySequence {
    KeySequence();
    KeySequence(Key key);
    KeySequence(std::initializer_list<Key> keys);
    KeySequence(const char* expression);
    
    std::size_t count() const;
    
    void append(Key key);
    void clear();
    
    const Key* begin() const;
    const Key* end() const;
    
    KeySequence withModifiersClosed() const;
    
  private:
    std::vector<Key> m_keys;
    
    Modifiers m_openModifiers;
  };
}
