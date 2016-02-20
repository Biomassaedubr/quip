#include "AttributeRange.hpp"

namespace quip {
  AttributeRange::AttributeRange (AttributeName name, std::size_t start, std::size_t length)
  : name(name)
  , start(start)
  , length(length) {
  }
}