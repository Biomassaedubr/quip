#pragma once

#include "Location.hpp"

#include <cstdint>

namespace quip {
  struct Selection {
    Selection (const Location & origin, const Location & extent);
    
    const Location & origin () const;
    const Location & extent () const;
    
    void setOrigin (const Location & location);
    void setExtent (const Location & location);
    
    const Location & lowerBound () const;
    const Location & upperBound () const;
    
    std::uint64_t height () const;
    
  private:
    Location m_origin;
    Location m_extent;
  };
}