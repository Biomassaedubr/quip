#pragma once

#include <cstdint>

namespace quip {
  // An unsigned 64-bit column and row position within a document.
  //
  // Locations start at (0, 0) and are not tied directly to any particular document.
  // Locations may refer to any cell within the addressible space, although the value
  // (0xFFFFFFFF, 0xFFFFFFFF) is reserved as an "invalid location" sentinel.
  struct Location {
    Location();
    Location(std::uint64_t column, std::uint64_t row);

    bool isValid() const;
    
    std::uint64_t column() const;
    std::uint64_t row() const;
    
    Location adjustBy(std::int64_t columnDelta, std::int64_t rowDelta) const;
    
    static Location invalid();
    
    friend void swap(Location& left, Location& right);

  private:
    std::uint64_t m_column;
    std::uint64_t m_row;
  };
  
  bool operator==(const Location& left, const Location& right);
  bool operator!=(const Location& left, const Location& right);
  bool operator<(const Location& left, const Location& right);
  bool operator<=(const Location& left, const Location& right);
  bool operator>(const Location& left, const Location& right);
  bool operator>=(const Location& left, const Location& right);
}
