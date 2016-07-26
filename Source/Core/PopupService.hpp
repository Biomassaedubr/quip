#pragma once

#include <string>

namespace quip {
  struct Location;
  
  // Provides functionality for creating, manipulating and destroying popup
  // views for an edit context.
  //
  // Popup views provide temporary adornment for a document and are useful for
  // things like hints, automatic-completion menus, and so on.
  struct PopupService {
    ~PopupService ();
    
    virtual void tick (double elapsedSeconds) = 0;
    
    virtual void createPopupAtLocation (const Location & location, const std::string & text) = 0;
  };
}