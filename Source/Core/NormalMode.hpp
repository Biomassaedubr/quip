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
    
    void selectThisWord (EditContext & context);
    void selectNextWord (EditContext & context);
    void selectPriorWord (EditContext & context);
    
    void selectThisLine (EditContext & context);
    void selectNextLine (EditContext & context);
    void selectPriorLine (EditContext & context);
    
    void enterJumpMode (EditContext & context);
    void enterEditMode (EditContext & context);
    void enterSearchMode (EditContext & context);
    
    void rotateSelectionForward (EditContext & context);
    void rotateSelectionBackward (EditContext & context);
    void collapseSelections (EditContext & context);
    
    void deleteSelections (EditContext & context);
    void changeSelections (EditContext & context);
    
    std::uint64_t m_virtualColumn;
  };
}