#include "Location.hpp"

#include <limits>
#include <utility>

namespace quip {
  namespace {
    constexpr std::uint64_t InvalidLocationColumn = std::numeric_limits<std::uint64_t>::max() - 1;
    constexpr std::uint64_t InvalidLocationRow = std::numeric_limits<std::uint64_t>::max() - 1;
  }
  
  Location::Location()
  : Location(0, 0) {
  }
  
  Location::Location(std::uint64_t column, std::uint64_t row)
  : m_column(column)
  , m_row(row) {
  }
  
  bool Location::isValid() const {
    return m_column != InvalidLocationColumn && m_row != InvalidLocationRow;
  }
  
  std::uint64_t Location::column() const {
    return m_column;
  }
  
  std::uint64_t Location::row() const {
    return m_row;
  }
  
  Location Location::adjustBy(std::int64_t columnDelta, std::int64_t rowDelta) const {
    // Clamp to zero (preventing wraparound).
    if (columnDelta < 0 && -columnDelta > m_column) {
      columnDelta = -m_column;
    }
    
    if (rowDelta < 0 && -rowDelta > m_row) {
      rowDelta = -m_row;
    }
    
    return Location(m_column + columnDelta, m_row + rowDelta);
  }
  
  Location Location::invalid() {
    return Location(InvalidLocationColumn, InvalidLocationRow);
  }
  
  bool operator==(const Location& left, const Location& right) {
    return left.column() == right.column() && left.row() == right.row();
  }
  
  bool operator!=(const Location& left, const Location& right) {
    return !(left == right);
  }
  
  bool operator<(const Location& left, const Location& right) {
    if (left.row() < right.row()) {
      return true;
    }
    
    if (left.row() == right.row() && left.column() < right.column()) {
      return true;
    }
    
    return false;
  }
  
  bool operator<=(const Location& left, const Location& right) {
    return (left == right) || (left < right);
  }
  
  bool operator>(const Location& left, const Location& right) {
    return !(left <= right);
  }
  
  bool operator>=(const Location& left, const Location& right) {
    return (left == right) || (left > right);
  }
  
  void swap(Location& left, Location& right) {
    using std::swap;
    swap(left.m_column, right.m_column);
    swap(left.m_row, right.m_row);
  }
}
