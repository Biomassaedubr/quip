#pragma once

#include "Mode.hpp"

namespace quip {
  struct EditContext;
  
  struct SearchMode : Mode {
    SearchMode ();
    
    std::string status () const override;
    
  protected:
    bool onUnmappedKey (Key key, const std::string & text, EditContext & context) override;
    
  private:
    void abortSearch (EditContext & context);
    void commitSearch (EditContext & context);
    
    std::string m_search;
  };
}
