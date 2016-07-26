#pragma once

#include <string>

namespace quip {
  // Provides functionality for manipulating the status area of a text view.
  struct StatusService {
    ~StatusService ();
    
    virtual void setStatus (const std::string & text) = 0;
    virtual void setFileType (const std::string & fileType) = 0;
    virtual void setLineCount (const std::size_t count) = 0;
  };
}