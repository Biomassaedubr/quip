#pragma once

#include "AttributeRange.hpp"

#include <regex>
#include <string>
#include <vector>

namespace quip {
  struct Syntax {
    Syntax ();
    
    std::vector<AttributeRange> highlight (const std::string & text) const;
    
  private:
    std::vector<std::string> m_keywords;
    std::regex m_keywordPattern;
  };
}