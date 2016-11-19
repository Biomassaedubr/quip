#pragma once

#include "AttributeRange.hpp"
#include "Coordinate.hpp"
#include "Extent.hpp"
#include "Location.hpp"

#include <string>
#include <vector>

namespace quip {
  struct Color;
  struct Rectangle;
  
  // Provides functionality for drawing text and indicators to a view.
  struct DrawingService {
    ~DrawingService ();
    
    Extent cellSize () const;

    Coordinate coordinateForLocationInFrame(const Location & location, const Rectangle & frame) const;
    Location locationForCoordinateInFrame(const Coordinate & coordinate, const Rectangle & frame) const;
    
    virtual void fillRectangle (const Rectangle & rectangle, const Color & color) = 0;
    
    virtual void drawUnderline (std::size_t row, std::size_t firstColumn, std::size_t lastColumn, const Color & color, const Rectangle & frame) = 0;
    virtual void drawBarBefore (const Location & location, const Color & color, const Rectangle & frame) = 0;
    
    void drawText (const std::string & text, const Coordinate& coordinate);
    
    virtual void drawText (const std::string & text, const Coordinate& coordinate, const std::vector<AttributeRange>& attributes) = 0;
    virtual Rectangle measureText (const std::string & text) = 0;
    
  protected:
    void setCellSize (Extent size);
    
  private:
    Extent m_cellSize;
  };
}
