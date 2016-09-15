#pragma once

#include "DrawingService.hpp"

#import <Cocoa/Cocoa.h>

namespace quip {
  struct DrawingServiceProvider : DrawingService {
    DrawingServiceProvider (const std::string & fontName, float fontSize);
    ~DrawingServiceProvider ();
  };
}