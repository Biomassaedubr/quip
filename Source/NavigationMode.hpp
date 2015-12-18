#pragma once

#include "Mode.hpp"

namespace quip {
  struct EditContext;
  
  struct NavigationMode : Mode {
    NavigationMode ();
    
    std::string status () const override;

  private:
    void selectBeforePrimaryOrigin (EditContext & context);
    void selectBelowPrimaryExtent (EditContext & context);
    void selectAfterPrimaryExtent (EditContext & context);
    void selectAbovePrimaryOrigin (EditContext & context);
    void enterInsertMode (EditContext & context);
  };
}