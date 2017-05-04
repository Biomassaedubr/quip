#pragma once

#include "Optional.hpp"

namespace quip {
  struct Document;
  struct Location;
  struct Selection;

  Optional<Selection> selectThisWord(const Document& document, const Selection& basis);
  Optional<Selection> selectNextWord(const Document& document, const Selection& basis);
  Optional<Selection> selectPriorWord(const Document& document, const Selection& basis);
  Optional<Selection> selectRemainingWord(const Document& document, const Selection& basis);
  Optional<Selection> selectThisLine(const Document& document, const Selection& basis);
  Optional<Selection> selectNextLine(const Document& document, const Selection& basis);
  Optional<Selection> selectPriorLine(const Document& document, const Selection& basis);
  Optional<Selection> selectBlocks(const Document& document, const Selection& basis);
  Optional<Selection> selectItem(const Document& document, const Selection& basis);
}
