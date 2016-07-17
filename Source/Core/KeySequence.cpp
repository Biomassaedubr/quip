#include "KeySequence.hpp"

namespace quip {
  KeySequence::KeySequence () {
  }
  
  KeySequence::KeySequence (Key key)
  : m_keys({key}) {
  }
  
  KeySequence::KeySequence (std::initializer_list<Key> keys)
  : m_keys(keys) {
  }
  
  std::size_t KeySequence::count () const {
    return m_keys.size();
  }
  
  void KeySequence::append (Key key) {
    m_keys.emplace_back(key);
  }
  
  void KeySequence::clear () {
    m_keys.clear();
  }
  
  const Key * KeySequence::begin () const {
    if (m_keys.size() == 0) {
      return nullptr;
    }
    
    return m_keys.data();
  }
  
  const Key * KeySequence::end () const {
    if (m_keys.size() == 0) {
      return nullptr;
    }
    
    return m_keys.data() + m_keys.size();
  }
}