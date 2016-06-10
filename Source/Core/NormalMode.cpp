#include "NormalMode.hpp"

#include "Classification.hpp"
#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "EditContext.hpp"
#include "EraseTransaction.hpp"
#include "Selection.hpp"
#include "SelectionSet.hpp"

namespace quip {
  NormalMode::NormalMode () {
    addMapping(Key::H, &NormalMode::selectBeforePrimaryOrigin);
    addMapping(Key::J, &NormalMode::selectBelowPrimaryExtent);
    addMapping(Key::K, &NormalMode::selectAbovePrimaryOrigin);
    addMapping(Key::L, &NormalMode::selectAfterPrimaryExtent);

    addMapping(Key::T, &NormalMode::selectThisWord);
    addMapping(Key::W, &NormalMode::selectNextWord);
    addMapping(Key::B, &NormalMode::selectPriorWord);

    addMapping(Key::A, &NormalMode::selectThisLine);

    addMapping(Key::R, &NormalMode::rotateSelectionForward);
    addMapping(Key::E, &NormalMode::rotateSelectionBackward);
    addMapping(Key::Z, &NormalMode::collapseSelections);

    addMapping(Key::I, &NormalMode::enterEditMode);
    addMapping(Key::S, &NormalMode::enterSearchMode);

    addMapping(Key::X, &NormalMode::deleteSelections);

    m_virtualColumn = 0;
  }
  
  std::string NormalMode::status () const {
    return "Normal";
  }
  
  void NormalMode::selectBeforePrimaryOrigin (EditContext & context) {
    if (context.document().isEmpty()) {
      return;
    }
    
    Location location = context.selections().primary().extent();
    if (location.column() == 0) {
      return;
    }
    
    Location target(location.column() - 1, location.row());
    m_virtualColumn = target.column();
    
    Selection result(target, target);
    context.selections().replace(result);
  }
  
  void NormalMode::selectBelowPrimaryExtent (EditContext & context) {
    if (context.document().isEmpty()) {
      return;
    }
    
    Location location = context.selections().primary().extent();
    if (location.row() + 1 == context.document().rows()) {
      return;
    }

    std::uint64_t column = location.column();
    m_virtualColumn = std::max(column, m_virtualColumn);
    column = std::max(column, m_virtualColumn);
    
    std::uint64_t row = location.row() + 1;
    if (column >= context.document().row(row).length()) {
      column = context.document().row(row).length() - 1;
    }

    Location target(column, row);
    Selection result(target, target);
    
    context.selections().replace(result);
  }

  void NormalMode::selectAfterPrimaryExtent (EditContext & context) {
    if (context.document().isEmpty()) {
      return;
    }
    
    Location location = context.selections().primary().extent();
    if (location.column() + 1 == context.document().row(location.row()).size()) {
      return;
    }
    
    Location target(location.column() + 1, location.row());
    m_virtualColumn = target.column();

    Selection result(target, target);
    context.selections().replace(result);
  }
  
  void NormalMode::selectAbovePrimaryOrigin (EditContext & context) {
    if (context.document().isEmpty()) {
      return;
    }
    
    Location location = context.selections().primary().extent();
    if (location.row() == 0) {
      return;
    }
    
    std::uint64_t column = location.column();
    m_virtualColumn = std::max(column, m_virtualColumn);
    column = std::max(column, m_virtualColumn);

    std::uint64_t row = location.row() - 1;
    if (column >= context.document().row(row).length()) {
      column = context.document().row(row).length() - 1;
    }
    
    Location target(column, row);
    Selection result(target, target);
    
    context.selections().replace(result);
  }
  
  void NormalMode::selectThisWord (EditContext & context) {
    context.selections().replace(classifyWord(context.document(), context.selections().primary().origin(), ClassificationFlags::This));
  }
  
  void NormalMode::selectNextWord (EditContext & context) {
    context.selections().replace(classifyWord(context.document(), context.selections().primary().extent(), ClassificationFlags::Next));
  }
  
  void NormalMode::selectPriorWord (EditContext & context) {
    context.selections().replace(classifyWord(context.document(), context.selections().primary().origin(), ClassificationFlags::Prior));
  }
  
  void NormalMode::selectThisLine (EditContext & context) {
    context.selections().replace(classifyLine(context.document(), context.selections().primary().origin(), ClassificationFlags::This));
  }
  
  void NormalMode::selectNextLine (EditContext & context) {
    context.selections().replace(classifyLine(context.document(), context.selections().primary().extent(), ClassificationFlags::Next));
  }
  
  void NormalMode::selectPriorLine (EditContext & context) {
    context.selections().replace(classifyLine(context.document(), context.selections().primary().origin(), ClassificationFlags::Prior));
  }
  
  void NormalMode::rotateSelectionForward (EditContext & context) {
    context.selections().rotateForward();
    context.controller().scrollToLocation.transmit(context.selections().primary().origin());
  }
  
  void NormalMode::rotateSelectionBackward (EditContext & context) {
    context.selections().rotateBackward();
    context.controller().scrollToLocation.transmit(context.selections().primary().origin());
  }
  
  void NormalMode::collapseSelections (EditContext & context) {
    context.selections().replace(context.selections().primary());
  }
  
  void NormalMode::enterEditMode (EditContext & context) {
    context.enterMode("EditMode");
  }
  
  void NormalMode::enterSearchMode (EditContext & context) {
    context.enterMode("SearchMode");
  }
  
  void NormalMode::deleteSelections (EditContext & context) {
    context.performTransaction(EraseTransaction::create(context.selections()));
  }
}