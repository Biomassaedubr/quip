#include "NavigationMode.hpp"

#include "EditContext.hpp"
#include "Selection.hpp"
#include "SelectionSet.hpp"

namespace quip {
  NavigationMode::NavigationMode () {
    addMapping(Key::H, &NavigationMode::selectBeforePrimaryOrigin);
    addMapping(Key::J, &NavigationMode::selectBelowPrimaryExtent);
    addMapping(Key::K, &NavigationMode::selectAbovePrimaryOrigin);
    addMapping(Key::L, &NavigationMode::selectAfterPrimaryExtent);
    addMapping(Key::I, &NavigationMode::enterInsertMode);
    addMapping(Key::S, &NavigationMode::enterSearchMode);
  }
  
  std::string NavigationMode::status () const {
    return "Navigate";
  }
  
  void NavigationMode::selectBeforePrimaryOrigin (EditContext & context) {
    Location location = context.selections().primary().extent();
    Location target(location.column() - 1, location.row());
    Selection result(target, target);
    
    context.selections().replace(result);
  }
  
  void NavigationMode::selectBelowPrimaryExtent (EditContext & context) {
    Location location = context.selections().primary().extent();
    Location target(location.column(), location.row() + 1);
    Selection result(target, target);
    
    context.selections().replace(result);
  }

  void NavigationMode::selectAfterPrimaryExtent (EditContext & context) {
    Location location = context.selections().primary().extent();
    Location target(location.column() + 1, location.row());
    Selection result(target, target);
    
    context.selections().replace(result);
  }
  
  void NavigationMode::selectAbovePrimaryOrigin (EditContext & context) {
    Location location = context.selections().primary().extent();
    Location target(location.column(), location.row() - 1);
    Selection result(target, target);
    
    context.selections().replace(result);
  }
  
  void NavigationMode::enterInsertMode (EditContext & context) {
    context.enterMode("InsertMode");
  }
  
  void NavigationMode::enterSearchMode (EditContext & context) {
    context.enterMode("SearchMode");
  }
}