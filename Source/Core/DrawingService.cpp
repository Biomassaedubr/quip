#include "DrawingService.hpp"

#include "Rectangle.hpp"

namespace quip {
  DrawingService::~DrawingService() {
  }
  
  Extent DrawingService::cellSize () const {
    return m_cellSize;
  }
  
  void DrawingService::setCellSize (Extent size) {
    m_cellSize = size;
  }
  
  Coordinate DrawingService::coordinateForLocationInFrame (const Location & location, const Rectangle & frame) const {
    float x = location.column() * m_cellSize.width();
    float y = frame.height() - (m_cellSize.height() * (location.row() + 1));
    return Coordinate(x, y);
  }
  
  Location DrawingService::locationForCoordinateInFrame (const Coordinate & coordinate, const Rectangle & frame) const {
    std::size_t column = static_cast<std::size_t>(coordinate.x / m_cellSize.width());
    std::size_t row = static_cast<std::size_t>((frame.height() - coordinate.y) / m_cellSize.height());
    return Location(column, row);
  }
  
  void DrawingService::drawText (const std::string & text, const Coordinate& coordinate) {
    std::vector<AttributeRange> attributes;
    drawText(text, coordinate, attributes);
  }
}
