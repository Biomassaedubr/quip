#pragma once

#include "Mode.hpp"

namespace quip {
  struct EditContext;
  struct KeyStroke;
  
  struct InsertMode : Mode {
    InsertMode ();
    
    std::string status () const override;
    
  protected:
    bool onUnmappedKey (const KeyStroke & keyStroke, EditContext & context) override;
    
  private:
    void commitInsert (EditContext & context);
  };
}