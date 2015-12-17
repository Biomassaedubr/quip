#include "Location.hpp"

namespace quip {
  Location::Location ()
  : Location(0, 0) {
  }
  
  Location::Location (std::uint64_t column, std::uint64_t row)
  : m_column(column)
  , m_row(row) {
  }
  
  std::uint64_t Location::column () const {
    return m_column;
  }
  
  std::uint64_t Location::row () const {
    return m_row;
  }
  
  Location Location::adjustBy (std::int64_t columnDelta, std::int64_t rowDelta) const {
    return Location(m_column + columnDelta, m_row + rowDelta);
  }
  
  bool operator== (const Location & left, const Location & right) {
    return left.column() == right.column() && left.row() == right.row();
  }
  
  bool operator!= (const Location & left, const Location & right) {
    return !(left == right);
  }
  
  bool operator< (const Location & left, const Location & right) {
    if (left.row() < right.row()) {
      return true;
    }
    
    if (left.row() == right.row() && left.column() < right.column()) {
      return true;
    }
    
    return false;
  }
  
  bool operator<= (const Location & left, const Location & right) {
    return (left == right) || (left < right);
  }
  
  bool operator> (const Location & left, const Location & right) {
    return !(left <= right);
  }
  
  bool operator>= (const Location & left, const Location & right) {
    return (left == right) || (left > right);
  }
}