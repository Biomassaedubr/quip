#pragma once

#include "Mode.hpp"

namespace quip {
  struct EditContext;
  
  struct EditMode : Mode {
    EditMode ();
    
    CursorStyle cursorStyle () const override;
    CursorFlags cursorFlags () const override;

    std::string status () const override;
    
  protected:
    bool allowsCounts () const override;
    
    bool onUnmappedKey (Key key, const std::string & text, EditContext & context) override;
    
  private:
    void commitInsert (EditContext & context);
  };
}
