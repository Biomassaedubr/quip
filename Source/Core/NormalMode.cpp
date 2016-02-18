#include "NormalMode.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "EditContext.hpp"
#include "Selection.hpp"
#include "SelectionSet.hpp"

namespace quip {
  NormalMode::NormalMode () {
    addMapping(Key::H, &NormalMode::selectBeforePrimaryOrigin);
    addMapping(Key::J, &NormalMode::selectBelowPrimaryExtent);
    addMapping(Key::K, &NormalMode::selectAbovePrimaryOrigin);
    addMapping(Key::L, &NormalMode::selectAfterPrimaryExtent);

    addMapping(Key::W, &NormalMode::selectNextWord);
    addMapping(Key::B, &NormalMode::selectPriorWord);

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
  
  void NormalMode::selectNextWord (EditContext & context) {
    DocumentIterator cursor = context.document().at(context.selections().primary().extent());
    DocumentIterator end = context.document().end();
    
    while(cursor != end && !std::isspace(*cursor)) {
      ++cursor;
    }
    
    while(cursor != end && std::isspace(*cursor)) {
      ++cursor;
    }
    
    Location origin = cursor.location();
    while(cursor != end && !std::isspace(*cursor)) {
      ++cursor;
    }
    
    context.selections().replace(Selection(origin, cursor.location()));
  }
  
  void NormalMode::selectPriorWord (EditContext & context) {
    DocumentIterator cursor = context.document().at(context.selections().primary().origin());
    DocumentIterator begin = context.document().begin();
    
    while(cursor != begin && !std::isspace(*cursor)) {
      --cursor;
    }
    
    while(cursor != begin && std::isspace(*cursor)) {
      --cursor;
    }
    
    Location origin = cursor.location();
    while(cursor != begin && !std::isspace(*cursor)) {
      --cursor;
    }
    
    context.selections().replace(Selection(cursor.location(), origin));
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
    context.document().erase(context.selections());
  }
}