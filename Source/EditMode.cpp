#include "EditMode.hpp"

#include "Document.hpp"
#include "EditContext.hpp"
#include "KeyStroke.hpp"

namespace quip {
  EditMode::EditMode () {
    addMapping(Key::Escape, &EditMode::commitInsert);
  }
  
  std::string EditMode::status () const {
    return "Edit";
  }
  
  bool EditMode::onUnmappedKey (const KeyStroke & keyStroke, EditContext & context) {
    if (keyStroke.key() == Key::Delete) {
      context.document().eraseBefore(context.selections());
      return true;
    } else if (keyStroke.text().size() > 0) {
      context.document().insert(context.selections(), keyStroke.text());
      return true;
    }
    
    return false;
  }
  
  void EditMode::commitInsert (EditContext & context) {
    context.leaveMode();
  }
}