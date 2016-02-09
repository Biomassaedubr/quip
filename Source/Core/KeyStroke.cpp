#include "KeyStroke.hpp"

namespace quip {
  KeyStroke::KeyStroke (Key key, const std::string & text)
  : m_key(key)
  , m_text(text) {
    if (key == Key::Return) {
      m_text = "\n";
    }
  }
  
  Key KeyStroke::key () const {
    return m_key;
  }
  
  const std::string & KeyStroke::text () const {
    return m_text;
  }
}
