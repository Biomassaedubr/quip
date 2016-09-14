#pragma once

#include "AttributeRange.hpp"

#include <regex>
#include <string>
#include <vector>

namespace quip {
  struct Document;
  
  struct SyntaxPattern {
    AttributeName attribute;
    std::string pattern;
  };
  
  struct Syntax {
    virtual std::vector<AttributeRange> parse (const std::string & text, const std::string & context) = 0;
  };
}