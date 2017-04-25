#pragma once

#include "AttributeRange.hpp"
#include "Color.hpp"
#include "DrawingService.hpp"
#include "Rectangle.hpp"

#include <unordered_map>

#import <Cocoa/Cocoa.h>

namespace quip {
  struct GlobalSettings;
  
  struct Highlight {
    CFDictionaryRef attributes;
    CGColorRef foregroundColor;
  };
  
  struct DrawingServiceProvider : DrawingService {
    DrawingServiceProvider (const GlobalSettings & settings);
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
    
    std::unordered_map<std::string, Highlight> m_highlightAttributes;
  };
}
