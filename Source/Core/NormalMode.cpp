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
    addMapping(Key::H, &NormalMode::doSelectBeforePrimaryOrigin);
    addMapping(Key::J, &NormalMode::doSelectBelowPrimaryExtent);
    addMapping(Key::K, &NormalMode::doSelectAbovePrimaryOrigin);
    addMapping(Key::L, &NormalMode::doSelectAfterPrimaryExtent);
    
    addMapping("TW", &NormalMode::doSelectThisWord);
    addMapping(Key::W, &NormalMode::doSelectNextWord);
    addMapping(Key::B, &NormalMode::doSelectPriorWord);

    addMapping("TL", &NormalMode::doSelectThisLine);

    addMapping("RF", &NormalMode::rotateSelectionForward);
    addMapping("RB", &NormalMode::rotateSelectionBackward);
    addMapping(Key::Z, &NormalMode::collapseSelections);

    addMapping(Key::F, &NormalMode::enterJumpMode);
    addMapping(Key::I, &NormalMode::enterEditMode);
    addMapping("/", &NormalMode::enterSearchMode);

    addMapping(Key::X, &NormalMode::deleteSelections);
    addMapping(Key::C, &NormalMode::changeSelections);

    m_virtualColumn = 0;
  }
  
  std::string NormalMode::status () const {
    return "Normal";
  }
  
  void NormalMode::doSelectBeforePrimaryOrigin (EditContext & context) {
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
    context.controller().scrollLocationIntoView.transmit(context.selections().primary().origin());
  }
  
  void NormalMode::doSelectBelowPrimaryExtent (EditContext & context) {
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
    context.controller().scrollLocationIntoView.transmit(context.selections().primary().origin());
  }

  void NormalMode::doSelectAfterPrimaryExtent (EditContext & context) {
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
    context.controller().scrollLocationIntoView.transmit(context.selections().primary().origin());
  }
  
  void NormalMode::doSelectAbovePrimaryOrigin (EditContext & context) {
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
    context.controller().scrollLocationIntoView.transmit(context.selections().primary().origin());
  }
  
  void NormalMode::doSelectThisWord (EditContext & context) {
    context.selections().replace(selectThisWord(context.document(), context.selections().primary()));
  }
  
  void NormalMode::doSelectNextWord (EditContext & context) {
    context.selections().replace(selectNextWord(context.document(), context.selections().primary()));
  }
  
  void NormalMode::doSelectPriorWord (EditContext & context) {
    context.selections().replace(selectPriorWord(context.document(), context.selections().primary()));
  }
  
  void NormalMode::doSelectThisLine (EditContext & context) {
    context.selections().replace(selectThisLine(context.document(), context.selections().primary()));
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
  
  void NormalMode::enterJumpMode (EditContext & context) {
    context.enterMode("JumpMode");
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
  
  void NormalMode::changeSelections (EditContext & context) {
    context.performTransaction(EraseTransaction::create(context.selections()));
    context.enterMode("EditMode");
  }
}
