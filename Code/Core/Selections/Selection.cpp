#include "Selection.hpp"

#include <algorithm>

namespace quip {
  Selection::Selection(const Location& location)
  : m_origin(location)
  , m_extent(location) {
  }
  
  Selection::Selection(const Location& origin, const Location& extent)
  : m_origin(origin)
  , m_extent(extent) {
    normalize();
  }
  
  Selection::Selection(const Selection& other)
  : m_origin(other.m_origin)
  , m_extent(other.m_extent) {
  }
  
  Selection::Selection(Selection&& other)
  : m_origin(other.m_origin)
  , m_extent(other.m_extent) {
  }
  
  Selection& Selection::operator=(const Selection& other) {
    m_origin = other.m_origin;
    m_extent = other.m_extent;
    return *this;
  }
  
  Selection& Selection::operator=(Selection&& other) {
    m_origin = other.m_origin;
    m_extent = other.m_extent;
    return *this;
  }

  const Location& Selection::origin() const {
    return m_origin;
  }
  
  const Location& Selection::extent() const {
    return m_extent;
  }

  std::uint64_t Selection::height() const {
    return m_extent.row() - m_origin.row() + 1;
  }
  
  void Selection::normalize() {
    if(m_origin > m_extent) {
      std::swap(m_origin, m_extent);
    }
  }
  
  bool operator==(const Selection& left, const Selection& right) {
    return left.origin() == right.origin()&& left.extent() == right.extent();
  }
  
  bool operator!=(const Selection& left, const Selection& right) {
    return !(left == right);
  }
  
  void swap(Selection& left, Selection& right) {
    using std::swap;
    swap(left.m_origin, right.m_origin);
    swap(left.m_extent, right.m_extent);
  }
}
