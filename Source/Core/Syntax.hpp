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
    Syntax (const std::string & name, std::initializer_list<SyntaxPattern> patterns);
    
    const std::string & name () const;
    
    std::vector<AttributeRange> highlight (const std::string & text) const;
    
    static Syntax * getSyntaxForExtention (const std::string & extension);
    static Syntax * getSyntaxForUnknown ();
    
  private:
    std::string m_name;
    std::vector<SyntaxPattern> m_patterns;
  };
}