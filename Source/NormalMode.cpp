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
    
    addMapping(Key::I, &NormalMode::enterInsertMode);
    addMapping(Key::S, &NormalMode::enterSearchMode);

    addMapping(Key::X, &NormalMode::deleteSelections);
  }
  
  std::string NormalMode::status () const {
    return "Normal";
  }
  
  void NormalMode::selectBeforePrimaryOrigin (EditContext & context) {
    Location location = context.selections().primary().extent();
    Location target(location.column() - 1, location.row());
    Selection result(target, target);
    
    context.selections().replace(result);
  }
  
  void NormalMode::selectBelowPrimaryExtent (EditContext & context) {
    Location location = context.selections().primary().extent();
    Location target(location.column(), location.row() + 1);
    Selection result(target, target);
    
    context.selections().replace(result);
  }

  void NormalMode::selectAfterPrimaryExtent (EditContext & context) {
    Location location = context.selections().primary().extent();
    Location target(location.column() + 1, location.row());
    Selection result(target, target);
    
    context.selections().replace(result);
  }
  
  void NormalMode::selectAbovePrimaryOrigin (EditContext & context) {
    Location location = context.selections().primary().extent();
    Location target(location.column(), location.row() - 1);
    Selection result(target, target);
    
    context.selections().replace(result);
  }
  
  void NormalMode::enterInsertMode (EditContext & context) {
    context.enterMode("InsertMode");
  }
  
  void NormalMode::enterSearchMode (EditContext & context) {
    context.enterMode("SearchMode");
  }
  
  void NormalMode::deleteSelections (EditContext & context) {
    context.document().erase(context.selections());
  }
}