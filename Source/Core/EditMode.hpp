#pragma once

#include "Mode.hpp"

namespace quip {
  struct EditContext;
  
  struct EditMode : Mode {
    EditMode ();
    
    CursorStyle cursorStyle () const override;
    CursorFlags cursorFlags () const override;

    std::string status () const override;
    
    static constexpr std::uint64_t InsertBehavior = 0;
    static constexpr std::uint64_t AppendBehavior = 1;
    
  protected:
    bool allowsCounts () const override;
    
    void onEnter (EditContext & context, std::uint64_t how) override;

    bool onUnmappedKey (Key key, const std::string & text, EditContext & context) override;
    
  private:
    void commitInsert (EditContext & context);
    
    bool m_useAppendBehavior;
  };
}
