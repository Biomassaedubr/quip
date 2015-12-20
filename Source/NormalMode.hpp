#pragma once

#include "Mode.hpp"

namespace quip {
  struct EditContext;
  
  struct NormalMode : Mode {
    NormalMode ();
    
    std::string status () const override;

  private:
    void selectBeforePrimaryOrigin (EditContext & context);
    void selectBelowPrimaryExtent (EditContext & context);
    void selectAfterPrimaryExtent (EditContext & context);
    void selectAbovePrimaryOrigin (EditContext & context);
    
    void enterEditMode (EditContext & context);
    void enterSearchMode (EditContext & context);
    
    void deleteSelections (EditContext & context);
  };
}