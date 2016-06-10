#pragma once

#include "AttributeRange.hpp"

#include <vector>

namespace quip {
  struct Document;
  
  // The "unknown" grammar is used as a fallback when no other grammar is appropriate for
  // a document. It is essentially a no-op parse, although for the time being it's also
  // used for testing.
  namespace UnknownGrammar {
    std::vector<AttributeRange> parse (const std::string & row, const std::string & context);
  }
}