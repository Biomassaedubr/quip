#include "JumpMode.hpp"

#include "Document.hpp"
#include "EditContext.hpp"
#include "Selection.hpp"
#include "SelectionDrawInfo.hpp"
#include "SelectionSet.hpp"
#include "Location.hpp"

namespace quip {
  JumpMode::JumpMode ()
  : m_row(0) {
  }
  
  std::string JumpMode::status () const {
    return "Jump Forward";
  }
  
  void JumpMode::onEnter (EditContext & context) {
    m_jumps.clear();

    const Selection & selection = context.selections().primary();
    Location start = selection.extent();
    m_row = start.row();
    
    const std::string line = context.document().row(m_row);
    
    // Scan forward looking for interesting characters to add to the jump set.
    std::vector<Selection> overlaySelections;
    bool priorWasWhitespace = std::isspace(line[start.column()]);
    for (std::uint64_t column = start.column() + 1; column < line.size(); ++column) {
      const char & character = line[column];
      bool thisIsWhitespace = std::isspace(character);
      
      if (priorWasWhitespace && !thisIsWhitespace) {
        if (m_jumps.find(character) == std::end(m_jumps)) {
          m_jumps[character] = column;
          overlaySelections.emplace_back(Location(column, m_row));
        }
      }
      
      priorWasWhitespace = thisIsWhitespace;
    }
    
    SelectionDrawInfo overlays;
    overlays.primaryColor = Color::Red;
    overlays.secondaryColor = Color::Red;
    overlays.selections.replace(SelectionSet(overlaySelections));
    overlays.flags = CursorFlags::None;
    overlays.style = CursorStyle::Underline;
    context.setOverlay("Jump", overlays);
  }
  
  void JumpMode::onExit (EditContext & context) {
    context.clearOverlay("Jump");
  }
  
  bool JumpMode::onUnmappedKey (const KeyStroke & keyStroke, EditContext & context) {
    if (keyStroke.key() == Key::Escape) {
      context.leaveMode();
    } else {
      char key = keyStroke.text()[0];
      std::map<char, std::uint64_t>::iterator cursor = m_jumps.find(key);
      if (cursor == std::end(m_jumps)) {
        return false;
      }
      
      std::uint64_t column = cursor->second;
      context.selections().replace(Selection(Location(column, m_row)));
      context.leaveMode();
    }
    
    return true;
  }
}