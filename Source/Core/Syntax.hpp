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
  
  typedef std::vector<AttributeRange> (*SyntaxParseFunction)(const std::string &, const std::string &);
  
  namespace Syntax {
    SyntaxParseFunction getSyntaxForExtention (const std::string & extension);
    SyntaxParseFunction getSyntaxForUnknown ();
  };
}