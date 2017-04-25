#pragma once

#import "QuipStatusView.h"

#include "StatusService.hpp"

#include <cstdint>

namespace quip {
  struct StatusServiceProvider : StatusService {
    StatusServiceProvider(QuipStatusView* view);
    
    void setStatus(const std::string& text) override;
    void setFileType(const std::string& fileType) override;
    void setLineCount(const std::size_t count) override;
                       
  private:
    QuipStatusView* m_view;
  };
}
