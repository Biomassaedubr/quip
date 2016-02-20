#pragma once

#include <cstddef>

namespace quip {
  enum AttributeName {
    Keyword = 0,
    AttributeCount
  };
  
  struct AttributeRange {
    AttributeName name;
    std::size_t start;
    std::size_t length;
    
    AttributeRange (AttributeName name, std::size_t start, std::size_t length);
  };
}