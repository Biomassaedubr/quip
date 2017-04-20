#include "KeySequence.hpp"

namespace quip {
  KeySequence::KeySequence() {
  }
  
  KeySequence::KeySequence(Key key)
  : m_keys({key}) {
  }
  
  KeySequence::KeySequence(std::initializer_list<Key> keys)
  : m_keys(keys) {
  }
  
  KeySequence::KeySequence(const char* expression) {
    const char* cursor = expression;
    while (cursor != nullptr && *cursor != 0) {
      if (*cursor == '<') {
        ++cursor;
        
        const char* anchor = cursor;
        while(*cursor != '>' && *cursor != '-') {
          ++cursor;
        }
        
        if (*cursor == '-') {
          // Read all modifiers.
          std::int32_t modifier = 0;
          while (anchor != cursor) {
            switch (*anchor) {
              case 'C':
                modifier |= static_cast<std::int32_t>(Key::ControlMask);
                break;
              case 'O':
                modifier |= static_cast<std::int32_t>(Key::OptionMask);
                break;
              case 'S':
                modifier |= static_cast<std::int32_t>(Key::ShiftMask);
                break;
              default:
                break;
            }
            
            ++anchor;
          }
          
          // Start the modifier sequence.
          append(static_cast<Key>(modifier | static_cast<std::int32_t>(Key::ModifierDownMask)));
          
          ++cursor;
          while (*cursor != '>') {
            append(keyFromCharacter(*cursor++));
          }
          
          // Complete the modifier sequence.
          append(static_cast<Key>(modifier | static_cast<std::int32_t>(Key::ModifierUpMask)));
        } else {
          std::string name(anchor, cursor);
          if (name == "ESC") {
            append(Key::Escape);
          }
        }
      } else {
        append(keyFromCharacter(*cursor));
      }
      
      ++cursor;
    }
  }
  
  std::size_t KeySequence::count() const {
    return m_keys.size();
  }
  
  void KeySequence::append(Key key) {
    m_keys.emplace_back(key);
    
    if (key == modifierDown(Key::ControlMask)) {
      m_openModifiers.control = true;
    }
    
    if (key == modifierDown(Key::OptionMask)) {
      m_openModifiers.option = true;
    }
    
    if (key == modifierDown(Key::ShiftMask)) {
      m_openModifiers.shift = true;
    }
  }
  
  void KeySequence::clear () {
    m_keys.clear();
    m_openModifiers.clear();
  }
  
  const Key* KeySequence::begin() const {
    if (m_keys.size() == 0) {
      return nullptr;
    }
    
    return m_keys.data();
  }
  
  const Key* KeySequence::end() const {
    if (m_keys.size() == 0) {
      return nullptr;
    }
    
    return m_keys.data() + m_keys.size();
  }
  
  KeySequence KeySequence::withModifiersClosed() const {
    KeySequence result(*this);
    if (m_openModifiers.control) {
      result.append(modifierUp(Key::ControlMask));
    }
    
    if (m_openModifiers.option) {
      result.append(modifierUp(Key::OptionMask));
    }
    
    if (m_openModifiers.shift) {
      result.append(modifierUp(Key::ShiftMask));
    }
    
    result.m_openModifiers.clear();
    return result;
  }
}
