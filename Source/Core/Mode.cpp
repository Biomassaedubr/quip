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
  
  bool Mode::processKeyEvent (Key key, const std::string & text, EditContext & context) {
    if (allowsCounts() && m_sequence.count() == 0 && keyIsNumber(key)) {
      m_count *= 10;
      m_count += numberFromKey(key);
    } else if (allowsRepeats() && m_sequence.count() == 0 && m_previousSequence.count() > 0 && key == Key::Period) {
      const MapTrieNode * node = m_mappings.find(m_previousSequence);
      if (node != nullptr) {
        for (std::uint32_t index = 0; index < std::max(1U, m_count); ++index) {
          node->handler()(context);
        }
      }
    } else {
      m_sequence.append(key);
    
      const MapTrieNode * node = m_mappings.find(m_sequence);
      if (node == nullptr) {
        m_sequence.clear();
        m_count = 0;
        return onUnmappedKey(key, text, context);
      } else if (node->handler() != nullptr) {
        m_previousSequence = m_sequence;
        m_sequence.clear();
        
        for (std::uint32_t index = 0; index < std::max(1U, m_count); ++index) {
          node->handler()(context);
        }
        
        m_count = 0;
      }
    }
    
    return true;
  }
  
  void Mode::enter (EditContext & context, std::uint64_t how) {
    onEnter(context, how);
  }
  
  void Mode::exit (EditContext & context) {
    onExit(context);
  }
  
  bool Mode::allowsRepeats () const {
    return true;
  }
  
  bool Mode::allowsCounts () const {
    return true;
  }
  
  void Mode::onEnter (EditContext & context, std::uint64_t how) {
  }
  
  void Mode::onExit (EditContext & context) {
  }
  
  bool Mode::onUnmappedKey (Key key, const std::string & text, EditContext & context) {
    context.popupService().createPopupAtLocation(context.selections().primary().origin(), "No mapping.");
    return false;
  }
}
