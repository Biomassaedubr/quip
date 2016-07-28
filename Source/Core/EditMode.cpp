#include "EditMode.hpp"

#include "Document.hpp"
#include "EditContext.hpp"
#include "EraseTransaction.hpp"
#include "InsertTransaction.hpp"
#include "KeyStroke.hpp"

namespace quip {
  namespace {
    void backspace (EditContext & context) {
      // Functionally, this is equivalent to erasing a selection that starts just before each
      // actual selection in the set.
      std::vector<Selection> adjusted;
      adjusted.reserve(context.selections().count());
      
      for (const Selection & selection : context.selections()) {
        Location origin = selection.origin();
        if (origin.column() == 0) {
          if (origin.row() == 0) {
            continue;
          }
          
          origin = Location(context.document().row(origin.row() - 1).length() - 1, origin.row() - 1);
        }
        else {
          origin = origin.adjustBy(-1, 0);
        }
        
        adjusted.emplace_back(Selection(origin));
      }
      
      SelectionSet set(adjusted);
      if (set.count() > 0) {
        context.document().erase(set);
        context.selections().replace(set);
      }
    }
  }
  
  EditMode::EditMode () {
    addMapping(Key::Escape, &EditMode::commitInsert);
  }
  
  CursorStyle EditMode::cursorStyle () const {
    return CursorStyle::VerticalBar;
  }
  
  CursorFlags EditMode::cursorFlags () const {
    return CursorFlags::Blink;
  }
  
  std::string EditMode::status () const {
    return "Edit";
  }
  
  bool EditMode::onUnmappedKey (const KeyStroke & keyStroke, EditContext & context) {
    switch (keyStroke.key()) {
      case Key::Tab:
        context.performTransaction(InsertTransaction::create(context.selections(), "  "));
        return true;
      case Key::Delete:
        backspace(context);
        return true;
      case Key::ArrowUp:
      case Key::ArrowDown:
      case Key::ArrowLeft:
      case Key::ArrowRight:
        break;
      default:
        if (keyStroke.text().size() > 0) {
          context.performTransaction(InsertTransaction::create(context.selections(), keyStroke.text()));
          return true;
        }
    }
    
    return false;
  }
  
  void EditMode::commitInsert (EditContext & context) {
    context.leaveMode();
  }
}