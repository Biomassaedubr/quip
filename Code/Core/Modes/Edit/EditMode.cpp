#include "EditMode.hpp"

#include "AppendTransaction.hpp"
#include "Document.hpp"
#include "EditContext.hpp"
#include "EraseTransaction.hpp"
#include "InsertTransaction.hpp"

namespace quip {
  namespace {
    void backspace (EditContext & context, bool isAppending) {
      // Functionally, this is equivalent to erasing a selection that starts just before each
      // actual selection in the set. If appending, it's equivalent to erasing the selection
      // collapsed to its origin.
      std::vector<Selection> adjusted;
      std::vector<Selection> replacement;
      adjusted.reserve(context.selections().count());
      
      std::size_t bias = isAppending ? 0 : 1;
      for (const Selection & selection : context.selections()) {
        Location origin = selection.origin();
        if (isAppending) {
          adjusted.emplace_back(Selection(origin));
        }
        else if (origin.column() == 0) {
          if (origin.row() == 0) {
            continue;
          }
          
          origin = Location(context.document().row(origin.row() - 1).length() - 1, origin.row() - 1);
        }
        else {
          origin = origin.adjustBy(-bias, 0);
        }
        
        adjusted.emplace_back(Selection(origin));
        
        Location replacementLocation = origin;
        if (isAppending) {
          replacementLocation = replacementLocation.adjustBy(-1, 0);
        }
        
        replacement.emplace_back(Selection(replacementLocation));
      }
      
      SelectionSet set(adjusted);
      if (set.count() > 0) {
        context.document().erase(set);
        context.selections().replace(SelectionSet(replacement));
      }
    }
  }
  
  EditMode::EditMode ()
  : m_useAppendBehavior(false) {
    addMapping(Key::Escape, &EditMode::commitInsert);
  }
  
  CursorStyle EditMode::cursorStyle () const {
    return m_useAppendBehavior ? CursorStyle::VerticalBarAtExtent : CursorStyle::VerticalBarAtOrigin;
  }
  
  CursorFlags EditMode::cursorFlags () const {
    return CursorFlags::Blink;
  }
  
  std::string EditMode::status () const {
    if (m_useAppendBehavior) {
      return "Edit (Append)";
    }
    
    return "Edit (Insert)";
  }
  
  bool EditMode::allowsCounts () const {
    return false;
  }
  
  bool EditMode::allowsRepeats () const {
    return false;
  }
  
  void EditMode::onEnter(EditContext & context, std::uint64_t how) {
    m_useAppendBehavior = how == AppendBehavior;
  }
  
  bool EditMode::onUnmappedKey (Key key, const std::string & text, EditContext & context) {
    switch (key) {
      case Key::Tab:
        if (m_useAppendBehavior) {
          context.performTransaction(AppendTransaction::create(context.selections(), "  "));
        } else {
          context.performTransaction(InsertTransaction::create(context.selections(), "  "));
        }
        return true;
      case Key::Delete:
        backspace(context, m_useAppendBehavior);
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
            
            if (m_useAppendBehavior) {
              context.performTransaction(AppendTransaction::create(context.selections(), indented));
            } else {
              context.performTransaction(InsertTransaction::create(context.selections(), indented));
            }
          } else {
            if (m_useAppendBehavior) {
              context.performTransaction(AppendTransaction::create(context.selections(), text));
            } else {
              context.performTransaction(InsertTransaction::create(context.selections(), text));
            }
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
