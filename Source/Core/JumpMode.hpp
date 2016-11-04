#pragma once

#include "Mode.hpp"

#include <map>

namespace quip {
  struct EditContext;
  
  struct JumpMode : Mode {
    JumpMode  ();
    
    std::string status () const override;
    
  protected:
    void onEnter (EditContext & context) override;
    void onExit (EditContext & context) override;
    
    bool onUnmappedKey (Key key, const std::string & text, EditContext & context) override;
    
  private:
    std::uint64_t m_row;
    std::map<char, std::uint64_t> m_jumps;
  };
}
