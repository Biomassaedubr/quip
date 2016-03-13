#pragma once

#include "AttributeRange.hpp"

#include <regex>
#include <string>
#include <vector>

namespace quip {
  struct SyntaxPattern {
    AttributeName attribute;
    std::string pattern;
  };
  
  struct Syntax {
    Syntax (std::initializer_list<SyntaxPattern> patterns);
    
    std::vector<AttributeRange> highlight (const std::string & text) const;
    
    static Syntax * forFileExtension (const std::string & extension);
    
  private:
    std::vector<SyntaxPattern> m_patterns;
  };
}