#include "InsertMode.hpp"

#include "Document.hpp"
#include "EditContext.hpp"
#include "KeyStroke.hpp"

namespace quip {
  InsertMode::InsertMode () {
    addMapping(Key::Escape, &InsertMode::commitInsert);
  }
  
  std::string InsertMode::status () const {
    return "Insert";
  }
  
  bool InsertMode::onUnmappedKey (const KeyStroke & keyStroke, EditContext & context) {
    if (keyStroke.text().size() > 0) {
      context.document().insert(context.selections(), keyStroke.text());
      return true;
    }
    
    return false;
  }
  
  void InsertMode::commitInsert (EditContext & context) {
    context.leaveMode();
  }
}