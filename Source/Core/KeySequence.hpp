#pragma once

#include "Key.hpp"

#include <vector>

namespace quip {
  // A sequence of key presses.
  //
  // A KeySequence is mainly used to associate input with commands in a Mode subclass.
  struct KeySequence {
    KeySequence ();
    KeySequence (Key key);
    KeySequence (std::initializer_list<Key> keys);
    
    std::size_t count () const;
    
    void append (Key key);
    void clear ();
    
    const Key * begin () const;
    const Key * end () const;
    
  private:
    std::vector<Key> m_keys;
  };
}