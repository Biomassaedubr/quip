#include "Mode.hpp"

#include "EditContext.hpp"

namespace quip {
  Mode::Mode ()
  : m_count(0) {
  }
  
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
    if (m_sequence.count() == 0 && keyIsNumber(keyStroke.key())) {
      m_count *= 10;
      m_count += numberFromKey(keyStroke.key());
    } else {
      m_sequence.append(keyStroke.key());
    
      const MapTrieNode * node = m_mappings.find(m_sequence);
      if (node == nullptr) {
        m_sequence.clear();
        m_count = 0;
        return onUnmappedKey(keyStroke, context);
      } else if (node->handler() != nullptr) {
        m_sequence.clear();
        
        for (std::uint32_t index = 0; index < std::max(1U, m_count); ++index) {
          node->handler()(context);
        }
        
        m_count = 0;
      }
    }
    
    return true;
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
    context.popupService().createPopupAtLocation(context.selections().primary().origin(), "No mapping.");
    return false;
  }
}