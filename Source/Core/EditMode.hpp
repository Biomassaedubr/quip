#pragma once

#include "Mode.hpp"

namespace quip {
  struct EditContext;
  struct KeyStroke;
  
  struct EditMode : Mode {
    EditMode ();
    
    CursorStyle cursorStyle () const override;
    CursorFlags cursorFlags () const override;

    std::string status () const override;
    
  protected:
    bool onUnmappedKey (const KeyStroke & keyStroke, EditContext & context) override;
    
  private:
    void commitInsert (EditContext & context);
  };
}