#pragma once

#include <functional>

namespace quip {
  struct Document;
  struct Location;
  struct Selection;
  
  enum ClassificationFlags {
    This,
    Next,
    Prior,
  };
  
  typedef std::function<Selection (const Document &, const Location &, ClassificationFlags)> ClassificationFunction;
  
  Selection selectThisWord (const Document & document, const Selection & basis);
  Selection selectNextWord (const Document & document, const Selection & basis);
  Selection selectPriorWord (const Document & document, const Selection & basis);
  Selection selectThisLine (const Document & document, const Selection & basis);
  Selection selectNextLine (const Document & document, const Selection & basis);
  Selection selectPriorLine (const Document & document, const Selection & basis);
}
