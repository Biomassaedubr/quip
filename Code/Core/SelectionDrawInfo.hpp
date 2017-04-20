#pragma once

#include "Color.hpp"
#include "CursorFlags.hpp"
#include "CursorStyle.hpp"
#include "SelectionSet.hpp"

namespace quip {
  struct SelectionDrawInfo {
    Color primaryColor;
    Color secondaryColor;
    CursorStyle style;
    CursorFlags flags;
    SelectionSet selections;
  };
}