#pragma once

#include <cstdint>

namespace quip {
  struct Location {
    Location ();
    Location (std::uint64_t column, std::uint64_t row);
    
    std::uint64_t column () const;
    std::uint64_t row () const;
    
    Location adjustBy (std::int64_t columnDelta, std::int64_t rowDelta) const;
    
    friend void swap (Location & left, Location & right);

  private:
    std::uint64_t m_column;
    std::uint64_t m_row;
  };
  
  bool operator== (const Location & left, const Location & right);
  bool operator!= (const Location & left, const Location & right);
  bool operator< (const Location & left, const Location & right);
  bool operator<= (const Location & left, const Location & right);
  bool operator> (const Location & left, const Location & right);
  bool operator>= (const Location & left, const Location & right);
}