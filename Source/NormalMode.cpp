#include "NormalMode.hpp"

#include "Document.hpp"
#include "EditContext.hpp"
#include "Selection.hpp"
#include "SelectionSet.hpp"

namespace quip {
  NormalMode::NormalMode () {
    addMapping(Key::H, &NormalMode::selectBeforePrimaryOrigin);
    addMapping(Key::J, &NormalMode::selectBelowPrimaryExtent);
    addMapping(Key::K, &NormalMode::selectAbovePrimaryOrigin);
    addMapping(Key::L, &NormalMode::selectAfterPrimaryExtent);

    addMapping(Key::R, &NormalMode::rotateSelectionForward);
    addMapping(Key::E, &NormalMode::rotateSelectionBackward);
    addMapping(Key::Z, &NormalMode::collapseSelections);

    addMapping(Key::I, &NormalMode::enterEditMode);
    addMapping(Key::S, &NormalMode::enterSearchMode);

    addMapping(Key::X, &NormalMode::deleteSelections);
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
    Selection result(target, target);
    
    context.selections().replace(result);
  }
  
  void NormalMode::selectBelowPrimaryExtent (EditContext & context) {
    Location location = context.selections().primary().extent();
    if (location.row() + 1 == context.document().rows()) {
      return;
    }
    
    Location target(location.column(), location.row() + 1);
    Selection result(target, target);
    
    context.selections().replace(result);
  }

  void NormalMode::selectAfterPrimaryExtent (EditContext & context) {
    Location location = context.selections().primary().extent();
    if (location.column() + 1 == context.document().row(location.row()).size()) {
      return;
    }
    
    Location target(location.column() + 1, location.row());
    Selection result(target, target);
    
    context.selections().replace(result);
  }
  
  void NormalMode::selectAbovePrimaryOrigin (EditContext & context) {
    Location location = context.selections().primary().extent();
    if (location.row() == 0) {
      return;
    }
    
    Location target(location.column(), location.row() - 1);
    Selection result(target, target);
    
    context.selections().replace(result);
  }
  
  void NormalMode::rotateSelectionForward (EditContext & context) {
    context.selections().rotateForward();
  }
  
  void NormalMode::rotateSelectionBackward (EditContext & context) {
    context.selections().rotateBackward();
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