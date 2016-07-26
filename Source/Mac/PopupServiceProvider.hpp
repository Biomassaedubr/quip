#pragma once

#import "QuipPopupView.h"
#import "QuipTextView.h"

#include "PopupService.hpp"

#include <cstdint>
#include <map>

namespace quip {  
  struct PopupServiceProvider : PopupService {
    PopupServiceProvider (QuipTextView * view);
    ~PopupServiceProvider ();
    
    void tick (double elapsedSeconds) override;
    
    PopupHandle createPopupAtLocation (const Location & location, const std::string & text) override;
    
  private:
    QuipTextView * m_view;
    std::map<PopupHandle, QuipPopupView *> m_popups;
  };
}
