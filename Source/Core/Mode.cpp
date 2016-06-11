#include "Mode.hpp"

#include "EditContext.hpp"

namespace quip {
  Mode::~Mode () {
  }
  
  CursorStyle Mode::cursorStyle () const {
    return CursorStyle::VerticalBlock;
  }
  
  CursorFlags Mode::cursorFlags () const {
    return CursorFlags::None;
  }
  
  std::string Mode::status () const {
    return "";
  }
  
  bool Mode::processKey (const KeyStroke & keyStroke, EditContext & context) {
    auto cursor = m_mappings.find(keyStroke.key());
    if (cursor != std::end(m_mappings)) {
      cursor->second(context);
      return true;
    }
    
    return onUnmappedKey(keyStroke, context);
  }
  
  void Mode::enter (EditContext & context) {
    onEnter(context);
  }
  
  void Mode::exit (EditContext & context) {
    onExit(context);
  }
  
  void Mode::onEnter (EditContext & context) {
  }
  
  void Mode::onExit (EditContext & context) {
  }
  
  bool Mode::onUnmappedKey (const KeyStroke & keyStroke, EditContext & context) {
    return false;
  }
}