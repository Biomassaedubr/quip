#pragma once

#include "Color.hpp"
#include "CursorStyle.hpp"
#include "SelectionSet.hpp"

namespace quip {
  struct SelectionDrawInfo {
    Color primaryColor;
    Color secondaryColor;
    CursorStyle style;
    SelectionSet selections;
  };
}