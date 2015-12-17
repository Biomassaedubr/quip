#pragma once

#include "Mode.hpp"

namespace quip {
  struct EditContext;
  struct KeyStroke;
  
  struct InsertMode : Mode {
    InsertMode ();
    
  protected:
    bool onUnmappedKey (const KeyStroke & keyStroke, EditContext & context) override;
    
  private:
    void commitInsert (EditContext & context);
  };
}