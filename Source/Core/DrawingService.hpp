#pragma once

#include "Coordinate.hpp"
#include "Extent.hpp"
#include "Location.hpp"
#include "Rectangle.hpp"

#include <string>

namespace quip {
  // Provides functionality for drawing text and indicators to a view.
  struct DrawingService {
    ~DrawingService ();
    
    Extent cellSize () const;

    Coordinate coordinateForLocationInFrame(const Location & location, const Rectangle & frame) const;
    Location locationForCoordinateInFrame(const Coordinate & coordinate, const Rectangle & frame) const;
  
  protected:
    void setCellSize (Extent size);
    
  private:
    Extent m_cellSize;
  };
}