#include "EditMode.hpp"

#include "Document.hpp"
#include "EditContext.hpp"
#include "EraseTransaction.hpp"
#include "InsertTransaction.hpp"

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
    return CursorStyle::VerticalBarAtOrigin;
  }
  
  CursorFlags EditMode::cursorFlags () const {
    return CursorFlags::Blink;
  }
  
  std::string EditMode::status () const {
    return m_status;
  }
  
  bool EditMode::allowsCounts () const {
    return false;
  }
  
  void EditMode::onEnter(EditContext & context, std::uint64_t how) {
    switch(how) {
      case InsertBehavior:
        m_status = "Edit (Insert)";
        break;
      case AppendBehavior:
        m_status = "Edit (Append)";
        break;
      default:
        m_status = "Edit";
        break;
    }
  }
  
  bool EditMode::onUnmappedKey (Key key, const std::string & text, EditContext & context) {
    switch (key) {
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
        if (text.size() > 0) {
          if (key == Key::Return) {
            std::vector<std::string> indented(context.selections().count(), text);
            for (std::uint32_t index = 0; index < context.selections().count(); ++index) {
              const Selection & selection = context.selections()[index];
              indented[index] += context.document().indentOfRow(selection.extent().row());
            }
            
            context.performTransaction(InsertTransaction::create(context.selections(), indented));
          } else {
            context.performTransaction(InsertTransaction::create(context.selections(), text));
          }
          
          context.controller().scrollLocationIntoView.transmit(context.selections().primary().extent());
          return true;
        }
    }
    
    return false;
  }
  
  void EditMode::commitInsert (EditContext & context) {
    context.leaveMode();
  }
}
