#pragma once

#include <string>

namespace quip {
  // Provides functionality for manipulating the status area of a text view.
  struct StatusService {
    ~StatusService ();
    
    virtual void setStatus (const std::string & text) = 0;
  };
}