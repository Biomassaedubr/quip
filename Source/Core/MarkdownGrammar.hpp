#pragma once

#include "AttributeRange.hpp"

#include <vector>

namespace quip {
  struct Document;
  
  namespace MarkdownGrammar {
    std::vector<AttributeRange> parse (const std::string & row, const std::string & context);
  }
}