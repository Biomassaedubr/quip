#include "NormalMode.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "EditContext.hpp"
#include "EditMode.hpp"
#include "EraseTransaction.hpp"
#include "Location.hpp"
#include "Selection.hpp"
#include "SelectionSet.hpp"
#include "Selector.hpp"

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
    addMapping("NL", &NormalMode::doSelectNextLine);
    addMapping("PL", &NormalMode::doSelectPriorLine);

    addMapping("RF", &NormalMode::rotateSelectionForward);
    addMapping("RB", &NormalMode::rotateSelectionBackward);
    addMapping(Key::Z, &NormalMode::collapseSelections);

    addMapping(Key::F, &NormalMode::enterJumpMode);
    addMapping("/", &NormalMode::enterSearchMode);
    addMapping("I", &NormalMode::enterEditModeByInserting);
    addMapping("<S-I>", &NormalMode::enterEditModeByInsertingAtStartOfLines);
    addMapping("A", &NormalMode::enterEditModeByAppending);
    addMapping("<S-A>", &NormalMode::enterEditModeByAppendingAtEndOfLines);

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
    context.controller().scrollToLocation.transmit(context.selections().primary().extent());
  }
  
  void NormalMode::doSelectNextWord (EditContext & context) {
    context.selections().replace(selectNextWord(context.document(), context.selections().primary()));
    context.controller().scrollLocationIntoView.transmit(context.selections().primary().extent());
  }
  
  void NormalMode::doSelectPriorWord (EditContext & context) {
    context.selections().replace(selectPriorWord(context.document(), context.selections().primary()));
    context.controller().scrollToLocation.transmit(context.selections().primary().extent());
  }
  
  void NormalMode::doSelectThisLine (EditContext & context) {
    context.selections().replace(selectThisLine(context.document(), context.selections().primary()));
    context.controller().scrollToLocation.transmit(context.selections().primary().extent());
  }
  
  void NormalMode::doSelectNextLine (EditContext & context) {
    context.selections().replace(selectNextLine(context.document(), context.selections().primary()));
    context.controller().scrollToLocation.transmit(context.selections().primary().extent());
  }
  
  void NormalMode::doSelectPriorLine (EditContext & context) {
    context.selections().replace(selectPriorLine(context.document(), context.selections().primary()));
    context.controller().scrollToLocation.transmit(context.selections().primary().extent());
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
  
  void NormalMode::enterSearchMode (EditContext & context) {
    context.enterMode("SearchMode");
  }
  
  void NormalMode::enterEditModeByInserting (EditContext & context) {
    context.enterMode("EditMode", EditMode::InsertBehavior);
  }
  
  void NormalMode::enterEditModeByInsertingAtStartOfLines (EditContext & context) {
    std::vector<Selection> adjusted;
    adjusted.reserve(context.selections().count());
    for(const Selection& selection : context.selections()) {
      Location location(0, selection.origin().row());
      adjusted.emplace_back(location);
    }
    
    context.selections().replace(SelectionSet(adjusted));
    context.enterMode("EditMode", EditMode::InsertBehavior);
  }
  
  void NormalMode::enterEditModeByAppending (EditContext & context) {
    context.enterMode("EditMode", EditMode::AppendBehavior);
  }
  
  void NormalMode::enterEditModeByAppendingAtEndOfLines (EditContext & context) {
    std::vector<Selection> adjusted;
    adjusted.reserve(context.selections().count());
    for(const Selection& selection : context.selections()) {
      Location location(context.document().row(selection.extent().row()).size() - 2, selection.extent().row());
      adjusted.emplace_back(location);
    }
    
    context.selections().replace(SelectionSet(adjusted));
    context.enterMode("EditMode", EditMode::AppendBehavior);
  }
  
  void NormalMode::deleteSelections (EditContext & context) {
    context.performTransaction(EraseTransaction::create(context.selections()));
  }
  
  void NormalMode::changeSelections (EditContext & context) {
    context.performTransaction(EraseTransaction::create(context.selections()));
    context.enterMode("EditMode");
  }
}
