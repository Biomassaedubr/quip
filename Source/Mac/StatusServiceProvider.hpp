#pragma once

#import "QuipStatusView.h"

#include "StatusService.hpp"

#include <cstdint>
#include <map>

namespace quip {
  struct StatusServiceProvider : StatusService {
    StatusServiceProvider (QuipStatusView * view);
    StatusServiceProvider ();
    
    void setStatus (const std::string & text) override;
    
  private:
    QuipStatusView * m_view;
  };
}
