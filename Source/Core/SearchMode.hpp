#pragma once

#include "Mode.hpp"

namespace quip {
  struct EditContext;
  struct KeyStroke;
  
  struct SearchMode : Mode {
    SearchMode ();
    
    std::string status () const override;
    
  protected:
    bool onUnmappedKey (const KeyStroke & keyStroke, EditContext & context) override;
    
  private:
    void commitSearch (EditContext & context);
    
    std::string m_search;
  };
}