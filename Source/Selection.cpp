#include "Selection.hpp"

#include <algorithm>

namespace quip {
  Selection::Selection (const Location & origin, const Location & extent)
  : m_origin(origin)
  , m_extent(extent) {
  }
  
  const Location & Selection::origin () const {
    return m_origin;
  }
  
  const Location & Selection::extent () const {
    return m_extent;
  }

  void Selection::setOrigin (const Location & location) {
    m_origin = location;
  }
  
  void Selection::setExtent (const Location & location) {
    m_extent = location;
  }
  
  const Location & Selection::lowerBound () const {
    return std::min(origin(), extent());
  }
  
  const Location & Selection::upperBound () const {
    return std::max(origin(), extent());
  }
  
  std::uint64_t Selection::height () const {
    return upperBound().row() - lowerBound().row() + 1;
  }
  
  bool operator== (const Selection & left, const Selection & right) {
    return left.origin() == right.origin() && left.extent() == right.extent();
  }
  
  bool operator!= (const Selection & left, const Selection & right) {
    return !(left == right);
  }
}