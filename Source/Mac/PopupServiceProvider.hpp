#pragma once

#import "QuipTextView.h"

#include "PopupService.hpp"

namespace quip {
  struct PopupServiceProvider : PopupService {
    PopupServiceProvider (QuipTextView * view);
    ~PopupServiceProvider ();
    
    void tick (double elapsedSeconds) override;
    
    void createPopupAtLocation (const Location & location, const std::string & text) override;
    
  private:
    QuipTextView * m_view;
  };
}
