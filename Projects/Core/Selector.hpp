#pragma once

#include "Optional.hpp"

namespace quip {
  struct Document;
  struct Location;
  struct Selection;

  Optional<Selection> selectThisOrNextWord(const Document& document, const Selection& basis);
  Optional<Selection> selectPriorWord(const Document& document, const Selection& basis);
  
  Optional<Selection> selectRemainingWord(const Document& document, const Selection& basis);
  
  Optional<Selection> selectThisOrNextLine(const Document& document, const Selection& basis);
  Optional<Selection> selectNextLine(const Document& document, const Selection& basis);
  Optional<Selection> selectPriorLine(const Document& document, const Selection& basis);
  Optional<Selection> selectBlocks(const Document& document, const Selection& basis);
  Optional<Selection> selectItem(const Document& document, const Selection& basis);
}
