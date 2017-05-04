#pragma once

#include "Mode.hpp"

namespace quip {
  struct EditContext;
  
  struct NormalMode : Mode {
    NormalMode();
    
    std::string status() const override;

  private:
    void doSelectBeforePrimaryOrigin(EditContext& context);
    void doSelectBelowPrimaryExtent(EditContext& context);
    void doSelectAfterPrimaryExtent(EditContext& context);
    void doSelectAbovePrimaryOrigin(EditContext& context);

    void doShiftSelectionExtentsLeft(EditContext& context);
    void doShiftSelectionExtentsDown(EditContext& context);
    void doShiftSelectionExtentsUp(EditContext& context);
    void doShiftSelectionExtentsRight(EditContext& context);

    void doShiftSelectionOriginsLeft(EditContext& context);
    void doShiftSelectionOriginsDown(EditContext& context);
    void doShiftSelectionOriginsUp(EditContext& context);
    void doShiftSelectionOriginsRight(EditContext& context);

    void doIncreaseSelectionIndentLevel(EditContext& context);
    void doDecreaseSelectionIndentLevel(EditContext& context);
    
    void doSelectWord(EditContext& context);
    void doSelectPriorWord(EditContext& context);
    void doSelectRemainingWord(EditContext& context);
    
    void doSelectThisLine(EditContext& context);
    void doSelectNextLine(EditContext& context);
    void doSelectPriorLine(EditContext& context);

    void doSelectBlocks(EditContext& context);

    void doSelectItems(EditContext& context);
    
    void enterJumpMode(EditContext& context);
    void enterSearchMode(EditContext& context);
    void enterEditModeByInserting(EditContext& context);
    void enterEditModeByInsertingAtStartOfLines(EditContext& context);
    void enterEditModeByAppending(EditContext& context);
    void enterEditModeByAppendingAtEndOfLines(EditContext& context);
    
    void rotateSelectionForward(EditContext& context);
    void rotateSelectionBackward(EditContext& context);
    void collapseSelections(EditContext& context);
    
    void deleteSelections(EditContext& context);
    void changeSelections(EditContext& context);
    
    std::uint64_t m_virtualColumn;
  };
}
