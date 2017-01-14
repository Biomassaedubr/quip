#pragma once

#include "AttributeRange.hpp"
#include "Color.hpp"
#include "DrawingService.hpp"
#include "Rectangle.hpp"

#import <Cocoa/Cocoa.h>

namespace quip {
  struct Highlight {
    CFDictionaryRef attributes;
    CGColorRef foregroundColor;
  };
  
  struct DrawingServiceProvider : DrawingService {
    DrawingServiceProvider (const std::string & fontName, float fontSize);
    ~DrawingServiceProvider ();
    
    void fillRectangle (const Rectangle & rectangle, const Color & color) override;
    
    void drawUnderline (std::size_t row, std::size_t firstColumn, std::size_t lastColumn, const Color & color, const Rectangle & frame) override;
    void drawBarBefore (const Location & location, const Color & color, const Rectangle & frame) override;
    void drawBarAfter (const Location & location, const Color & color, const Rectangle & frame) override;
    
    void drawText (const std::string & text, const Coordinate & coordinate, const std::vector<AttributeRange> & attributes) override;
    Rectangle measureText (const std::string & text) override;
    
  private:
    CTFontRef m_font;
    CFDictionaryRef m_fontAttributes;
    
    Highlight m_highlightAttributes[AttributeCount];
  };
}
