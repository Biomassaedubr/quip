#include "Classification.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "Selection.hpp"

namespace quip {
  namespace {
    bool isWordCharacter (char character) {
      return std::isalnum(character);
    }
  }
  
  Selection selectThisWord (const Document & document, const Selection & basis) {
    // Move backwards until after a space.
    DocumentIterator origin = document.at(basis.origin());
    while (origin != document.begin() && isWordCharacter(*origin)) {
      --origin;
    }
    ++origin;
    
    // Move forwards until before a space.
    DocumentIterator extent = document.at(basis.extent());
    while (extent != document.end() && isWordCharacter(*extent)) {
      ++extent;
    }
    
    // ...then include the whitespace range until the next word character.
    while(extent != document.end() && std::isspace(*extent)) {
      ++extent;
    }
    
    // Don't actually include that last character.
    if (extent != document.end()) {
      --extent;
    }
    
    return Selection(origin.location(), extent.location());
  }
  
  Selection selectNextWord (const Document & document, const Selection & basis) {
    // Move until a non-word character.
    DocumentIterator cursor = document.at(basis.extent());
    while (cursor != document.end() && isWordCharacter(*cursor)) {
      ++cursor;
    }
    
    if (cursor == document.end()) {
      return basis;
    }
    
    // Then move until a word character.
    while (cursor != document.end() && !isWordCharacter(*cursor)) {
      ++cursor;
    }
    
    if (cursor == document.end()) {
      return basis;
    }
    
    // Save the origin, and move to the next non-word character...
    Location origin = cursor.location();
    while (cursor != document.end() && isWordCharacter(*cursor)) {
      ++cursor;
    }
    
    // ...then include the whitespace range until the next word character.
    while(cursor != document.end() && std::isspace(*cursor)) {
      ++cursor;
    }
    
    // Don't actually include that last character.
    if (cursor != document.end()) {
      --cursor;
    }
    
    return Selection(origin, cursor.location());
  }
  
  Selection selectPriorWord (const Document & document, const Selection & basis) {
    // Move until a non-word character.
    DocumentIterator cursor = document.at(basis.origin());
    while (cursor != document.begin() && isWordCharacter(*cursor)) {
      --cursor;
    }
    
    // Then move until a word character.
    while (cursor != document.begin() && !isWordCharacter(*cursor)) {
      --cursor;
    }
    
    // Save the extent, and move to the next non-word character...
    return selectThisWord(document, Selection(cursor.location()));
  }
  
  Selection selectThisLine (const Document & document, const Selection & basis) {
    Location origin(0, basis.origin().row());
    
    std::uint64_t row = basis.extent().row();
    Location extent(document.row(row).size() - 1, row);
    return Selection(origin, extent);
  }
  
  Selection selectNextLine (const Document & document, const Selection & basis) {
    std::uint64_t row = basis.extent().row();
    if (row + 1 < document.rows()) {
      ++row;
      Location origin(0, row);
      Location extent(document.row(row).size() - 1, row);
      return Selection(origin, extent);
    }
    
    return selectThisLine(document, basis);
  }
  
  Selection selectPriorLine (const Document & document, const Selection & basis) {
    std::uint64_t row = basis.origin().row();
    if (row > 0) {
      --row;
      Location origin(0, row);
      Location extent(document.row(row).size() - 1, row);
      return Selection(origin, extent);
    }
    
    return selectThisLine(document, basis);
  }
}
