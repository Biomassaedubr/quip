#include "EditMode.hpp"

#include "Document.hpp"
#include "EditContext.hpp"
#include "KeyStroke.hpp"

namespace quip {
  EditMode::EditMode () {
    addMapping(Key::Escape, &EditMode::commitInsert);
  }
  
  CursorStyle EditMode::cursorStyle () const {
    return CursorStyle::VerticalBar;
  }
  
  std::string EditMode::status () const {
    return "Edit";
  }
  
  bool EditMode::onUnmappedKey (const KeyStroke & keyStroke, EditContext & context) {
    switch (keyStroke.key()) {
      case Key::Delete:
        context.document().eraseBefore(context.selections());
        return true;
      case Key::ArrowUp:
      case Key::ArrowDown:
      case Key::ArrowLeft:
      case Key::ArrowRight:
        break;
      default:
        if (keyStroke.text().size() > 0) {
          context.document().insert(context.selections(), keyStroke.text());
          return true;
        }
    }
    
    return false;
  }
  
  void EditMode::commitInsert (EditContext & context) {
    context.leaveMode();
  }
}