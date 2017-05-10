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
  NormalMode::NormalMode() {
    addMapping("H", &NormalMode::doSelectBeforePrimaryOrigin);
    addMapping("J", &NormalMode::doSelectBelowPrimaryExtent);
    addMapping("K", &NormalMode::doSelectAbovePrimaryOrigin);
    addMapping("L", &NormalMode::doSelectAfterPrimaryExtent);
    
    addMapping("<S-H>", &NormalMode::doShiftSelectionExtentsLeft);
    addMapping("<S-J>", &NormalMode::doShiftSelectionExtentsDown);
    addMapping("<S-K>", &NormalMode::doShiftSelectionExtentsUp);
    addMapping("<S-L>", &NormalMode::doShiftSelectionExtentsRight);

    addMapping("<O-H>", &NormalMode::doShiftSelectionOriginsLeft);
    addMapping("<O-J>", &NormalMode::doShiftSelectionOriginsDown);
    addMapping("<O-K>", &NormalMode::doShiftSelectionOriginsUp);
    addMapping("<O-L>", &NormalMode::doShiftSelectionOriginsRight);
    
    addMapping("<S-.>", &NormalMode::doIncreaseSelectionIndentLevel);
    addMapping("<S-,>", &NormalMode::doDecreaseSelectionIndentLevel);
    
    addMapping("W", &NormalMode::doSelectWord);
    addMapping("B", &NormalMode::doSelectPriorWord);
    addMapping("RW", &NormalMode::doSelectRemainingWord);
    
    addMapping("L", &NormalMode::doSelectThisLine);
    addMapping("NL", &NormalMode::doSelectNextLine);
    addMapping("PL", &NormalMode::doSelectPriorLine);
    
    addMapping("<S-B>", &NormalMode::doSelectBlocks);
    addMapping("<O-I>", &NormalMode::doSelectItems);
    
    addMapping("RF", &NormalMode::rotateSelectionForward);
    addMapping("RB", &NormalMode::rotateSelectionBackward);
    addMapping("Z", &NormalMode::collapseSelections);
    
    addMapping("F", &NormalMode::enterJumpMode);
    addMapping("/", &NormalMode::enterSearchMode);
    addMapping("I", &NormalMode::enterEditModeByInserting);
    addMapping("<S-I>", &NormalMode::enterEditModeByInsertingAtStartOfLines);
    addMapping("A", &NormalMode::enterEditModeByAppending);
    addMapping("<S-A>", &NormalMode::enterEditModeByAppendingAtEndOfLines);
    
    addMapping("X", &NormalMode::deleteSelections);
    addMapping("C", &NormalMode::changeSelections);
    
    m_virtualColumn = 0;
  }
  
  std::string NormalMode::status() const {
    return "Normal";
  }
  
  void NormalMode::doSelectBeforePrimaryOrigin(EditContext& context) {
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
  
  void NormalMode::doSelectBelowPrimaryExtent(EditContext& context) {
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
  
  void NormalMode::doSelectAfterPrimaryExtent(EditContext& context) {
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
  
  void NormalMode::doSelectAbovePrimaryOrigin(EditContext& context) {
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
  
  void NormalMode::doShiftSelectionExtentsLeft(EditContext& context) {
    Document& document = context.document();
    SelectionSet& selections = context.selections();
    std::vector<Selection> results;
    results.reserve(selections.count());
    
    for(const Selection& selection : selections) {
      DocumentIterator iterator = document.from(selection.extent());
      --iterator;
      
      if (iterator.location() < selection.origin()) {
        results.emplace_back(selection);
      } else {
        results.emplace_back(selection.origin(), iterator.location());
      }
    }
    
    selections.replace(SelectionSet(results));
  }
  
  void NormalMode::doShiftSelectionExtentsDown(EditContext& context) {
    Document& document = context.document();
    SelectionSet& selections = context.selections();
    std::vector<Selection> results;
    results.reserve(selections.count());
    
    for(const Selection& selection : selections) {
      if (selection.extent().row() == document.rows() - 1) {
        results.emplace_back(selection);
      }
      
      Location target = selection.extent().adjustBy(0, 1);
      if (target.column() > document.row(target.row()).size()) {
        target = Location(document.row(target.row()).size() - 1, target.row());
      }

      results.emplace_back(selection.origin(), target);
    }
    
    selections.replace(SelectionSet(results));
  }
  
  void NormalMode::doShiftSelectionExtentsUp(EditContext& context) {
    Document& document = context.document();
    SelectionSet& selections = context.selections();
    std::vector<Selection> results;
    results.reserve(selections.count());
    
    for(const Selection& selection : selections) {
      if (selection.extent().row() == 0) {
        results.emplace_back(selection);
      }
      
      Location target = selection.extent().adjustBy(0, -1);
      if (target.column() > document.row(target.row()).size()) {
        target = Location(document.row(target.row()).size() - 1, target.row());
      }
      
      if (target < selection.origin()) {
        results.emplace_back(selection);
      } else {
        results.emplace_back(selection.origin(), target);
      }
    }
    
    selections.replace(SelectionSet(results));
  }
  
  void NormalMode::doShiftSelectionExtentsRight(EditContext& context) {
    Document& document = context.document();
    SelectionSet& selections = context.selections();
    std::vector<Selection> results;
    results.reserve(selections.count());
    
    for(const Selection& selection : selections) {
      DocumentIterator iterator = document.from(selection.extent());
      ++iterator;
      
      results.emplace_back(selection.origin(), iterator.location());
    }
    
    selections.replace(SelectionSet(results));
  }
  
  void NormalMode::doShiftSelectionOriginsLeft(EditContext& context) {
    Document& document = context.document();
    SelectionSet& selections = context.selections();
    std::vector<Selection> results;
    results.reserve(selections.count());
    
    for(const Selection& selection : selections) {
      DocumentIterator iterator = document.from(selection.origin());
      if (iterator == document.begin()) {
        results.emplace_back(selection);
      } else {
        --iterator;
        results.emplace_back(iterator.location(), selection.extent());
      }
    }
    
    selections.replace(SelectionSet(results));
  }
  
  void NormalMode::doShiftSelectionOriginsDown(EditContext& context) {
    Document& document = context.document();
    SelectionSet& selections = context.selections();
    std::vector<Selection> results;
    results.reserve(selections.count());
    
    for(const Selection& selection : selections) {
      if (selection.origin().row() == document.rows() - 1) {
        results.emplace_back(selection);
      } else {
        Location target = selection.origin().adjustBy(0, 1);
        if (target.column() > document.row(target.row()).size()) {
          target = Location(document.row(target.row()).size() - 1, target.row());
        }
        
        results.emplace_back(target, selection.extent());
      }
    }
    
    selections.replace(SelectionSet(results));
  }
  
  void NormalMode::doShiftSelectionOriginsUp(EditContext& context) {
    Document& document = context.document();
    SelectionSet& selections = context.selections();
    std::vector<Selection> results;
    results.reserve(selections.count());
    
    for(const Selection& selection : selections) {
      if (selection.origin().row() == 0) {
        results.emplace_back(selection);
      } else {
        Location target = selection.origin().adjustBy(0, -1);
        if (target.column() > document.row(target.row()).size()) {
          target = Location(document.row(target.row()).size() - 1, target.row());
        }
        
        if (target > selection.extent()) {
          results.emplace_back(selection);
        } else {
          results.emplace_back(target, selection.origin());
        }
      }
    }
    
    selections.replace(SelectionSet(results));
  }
  
  void NormalMode::doShiftSelectionOriginsRight(EditContext& context) {
    Document& document = context.document();
    SelectionSet& selections = context.selections();
    std::vector<Selection> results;
    results.reserve(selections.count());
    
    for(const Selection& selection : selections) {
      DocumentIterator iterator = document.from(selection.origin());
      ++iterator;
      
      if (iterator.location() <= selection.extent()) {
        results.emplace_back(iterator.location(), selection.extent());
      } else {
        results.emplace_back(selection);
      }
    }
    
    selections.replace(SelectionSet(results));
  }
  
  void NormalMode::doIncreaseSelectionIndentLevel(EditContext& context) {
    Document& document = context.document();
    SelectionSet& selections = context.selections();
    std::vector<Selection> results;
    results.reserve(selections.count());
    for (const Selection& selection : selections) {
      Location target(0, selection.origin().row());
      document.insert(Selection(target), "  ");
      
      results.emplace_back(selection.origin().adjustBy(2, 0), selection.extent().adjustBy(2, 0));
    }
    
    selections.replace(SelectionSet(results));
  }
  
  void NormalMode::doDecreaseSelectionIndentLevel(EditContext& context) {
    Document& document = context.document();
    SelectionSet& selections = context.selections();
    std::vector<Selection> results;
    results.reserve(selections.count());
    for (const Selection& selection : selections) {
      std::uint64_t row = selection.origin().row();
      const std::string& text = document.row(row);
      
      std::uint64_t size = 0;
      if (!std::isspace(text[size])) {
        results.emplace_back(selection);
        continue;
      }
      
      while (std::isspace(text[size]) && size < 1) {
        ++size;
      }
      
      Location start(0, row);
      Location end(size, row);
      document.erase(Selection(start, end));
      
      results.emplace_back(selection.origin().adjustBy(-size - 1, 0), selection.extent().adjustBy(-size - 1, 0));
    }
    
    selections.replace(SelectionSet(results));
  }
  
  void NormalMode::doSelectWord(EditContext& context) {
    Optional<Selection> result = selectWord(context.document(), context.selections().primary());
    if (result.has_value()) {
      context.selections().replace(result.value());
      context.controller().scrollToLocation.transmit(context.selections().primary().extent());
    }
  }
  
  void NormalMode::doSelectPriorWord(EditContext& context) {
    Optional<Selection> result = selectPriorWord(context.document(), context.selections().primary());
    if (result.has_value()) {
      context.selections().replace(result.value());
      context.controller().scrollToLocation.transmit(context.selections().primary().extent());
    }
  }
  
  void NormalMode::doSelectRemainingWord(EditContext& context) {
    Optional<Selection> result = selectRemainingWord(context.document(), context.selections().primary());
    if (result.has_value()) {
      context.selections().replace(result.value());
      context.controller().scrollToLocation.transmit(context.selections().primary().extent());
    }
  }
  
  void NormalMode::doSelectThisLine(EditContext& context) {
    Optional<Selection> result = selectThisLine(context.document(), context.selections().primary());
    if (result.has_value()) {
      context.selections().replace(result.value());
      context.controller().scrollToLocation.transmit(context.selections().primary().extent());
    }
  }
  
  void NormalMode::doSelectNextLine(EditContext& context) {
    Optional<Selection> result = selectNextLine(context.document(), context.selections().primary());
    if (result.has_value()) {
      context.selections().replace(result.value());
      context.controller().scrollToLocation.transmit(context.selections().primary().extent());
    }
  }
  
  void NormalMode::doSelectPriorLine(EditContext& context) {
    Optional<Selection> result = selectPriorLine(context.document(), context.selections().primary());
    if (result.has_value()) {
      context.selections().replace(result.value());
      context.controller().scrollToLocation.transmit(context.selections().primary().extent());
    }
  }
  
  void NormalMode::doSelectBlocks(EditContext& context) {
    Optional<Selection> result = selectBlocks(context.document(), context.selections().primary());
    if (result.has_value()) {
      context.selections().replace(result.value());
      context.controller().scrollLocationIntoView.transmit(context.selections().primary().extent());
    }
  }
  
  void NormalMode::doSelectItems(EditContext& context) {
    Optional<Selection> result = selectItem(context.document(), context.selections().primary());
    if (result.has_value()) {
      context.selections().replace(result.value());
      context.controller().scrollLocationIntoView.transmit(context.selections().primary().extent());
    }
  }
  
  void NormalMode::rotateSelectionForward(EditContext& context) {
    context.selections().rotateForward();
    context.controller().scrollToLocation.transmit(context.selections().primary().origin());
  }
  
  void NormalMode::rotateSelectionBackward(EditContext& context) {
    context.selections().rotateBackward();
    context.controller().scrollToLocation.transmit(context.selections().primary().origin());
  }
  
  void NormalMode::collapseSelections(EditContext& context) {
    context.selections().replace(context.selections().primary());
  }
  
  void NormalMode::enterJumpMode(EditContext& context) {
    context.enterMode("JumpMode");
  }
  
  void NormalMode::enterSearchMode(EditContext& context) {
    context.enterMode("SearchMode");
  }
  
  void NormalMode::enterEditModeByInserting(EditContext& context) {
    context.enterMode("EditMode", EditMode::InsertBehavior);
  }
  
  void NormalMode::enterEditModeByInsertingAtStartOfLines(EditContext& context) {
    std::vector<Selection> adjusted;
    adjusted.reserve(context.selections().count());
    for (const Selection& selection : context.selections()) {
      Location location(0, selection.origin().row());
      adjusted.emplace_back(location);
    }
    
    context.selections().replace(SelectionSet(adjusted));
    context.enterMode("EditMode", EditMode::InsertBehavior);
  }
  
  void NormalMode::enterEditModeByAppending(EditContext& context) {
    context.enterMode("EditMode", EditMode::AppendBehavior);
  }
  
  void NormalMode::enterEditModeByAppendingAtEndOfLines(EditContext& context) {
    std::vector<Selection> adjusted;
    adjusted.reserve(context.selections().count());
    for (const Selection& selection : context.selections()) {
      Location location(context.document().row(selection.extent().row()).size() - 2, selection.extent().row());
      adjusted.emplace_back(location);
    }
    
    context.selections().replace(SelectionSet(adjusted));
    context.enterMode("EditMode", EditMode::AppendBehavior);
  }
  
  void NormalMode::deleteSelections(EditContext& context) {
    context.performTransaction(EraseTransaction::create(context.selections()));
  }
  
  void NormalMode::changeSelections(EditContext& context) {
    context.performTransaction(EraseTransaction::create(context.selections()));
    context.enterMode("EditMode");
  }
}
