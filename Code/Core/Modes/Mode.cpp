#include "Mode.hpp"

#include "EditContext.hpp"

namespace quip {
  Mode::Mode()
  : m_count(0) {
  }
  
  Mode::~Mode() {
  }
  
  CursorStyle Mode::cursorStyle() const {
    return CursorStyle::VerticalBlock;
  }
  
  CursorFlags Mode::cursorFlags() const {
    return CursorFlags::None;
  }
  
  std::string Mode::status() const {
    return "";
  }

  bool Mode::processKeyEvent(Key key, Modifiers modifiers, EditContext& context) {
    m_sequence.append(key);
    return true;
  }
  
  bool Mode::processKeyEvent(Key key, Modifiers modifiers, const std::string& text, EditContext& context) {
    if (allowsCounts() && m_sequence.count() == 0 && keyIsNumber(key)) {
      m_count *= 10;
      m_count += numberFromKey(key);
    } else if (allowsRepeats() && m_sequence.count() == 0 && m_previousSequence.count() > 0 && key == Key::Period) {
      const MapTrieNode* node = m_mappings.find(m_previousSequence);
      if (node != nullptr) {
        for (std::uint32_t index = 0; index < std::max(1U, m_count); ++index) {
          node->handler()(context);
        }
      }
    } else {
      // If any modifiers are present for this keystroke that were not present in the prior keystroke,
      // add them into the sequence.
      if (modifiers.shift && !m_modifiers.shift) {
        m_sequence.append(modifierDown(Key::ShiftMask));
      }
      
      m_sequence.append(key);
      
      // Copy the sequence, closing any open modifiers. This allows the sequence to be looked up
      // in the mapping trie.
      KeySequence checked(m_sequence.withModifiersClosed());
      const MapTrieNode* node = m_mappings.find(checked);
      if (node == nullptr) {
        m_sequence.clear();
        m_count = 0;
        return onUnmappedKey(key, text, context);
      } else if (node->handler() != nullptr) {
        m_previousSequence = checked;
        m_sequence.clear();
        
        for (std::uint32_t index = 0; index < std::max(1U, m_count); ++index) {
          node->handler()(context);
        }
        
        m_count = 0;
        
        // Reset the modifier state as well; if the keys are still held, that will
        // bubble through in the next actual keypress and the code at the top of
        // this block will insert the appropriate opening sequences.
        m_modifiers.clear();
      }
    }
    
    return true;
  }
  
  void Mode::enter(EditContext& context, std::uint64_t how) {
    onEnter(context, how);
  }
  
  void Mode::exit(EditContext& context) {
    onExit(context);
  }
  
  bool Mode::allowsRepeats() const {
    return true;
  }
  
  bool Mode::allowsCounts() const {
    return true;
  }
  
  void Mode::onEnter(EditContext& context, std::uint64_t how) {
  }
  
  void Mode::onExit(EditContext& context) {
  }
  
  bool Mode::onUnmappedKey (Key key, const std::string& text, EditContext& context) {
    context.popupService().createPopupAtLocation(context.selections().primary().origin(), "No mapping.");
    return false;
  }
}
