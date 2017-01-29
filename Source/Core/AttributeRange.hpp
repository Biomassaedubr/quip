#pragma once

#include <cstddef>
#include <string>

namespace quip {
  struct AttributeRange {
    std::string name;
    std::size_t start;
    std::size_t length;
    
    AttributeRange (const std::string& name, std::size_t start, std::size_t length);
  };
}
