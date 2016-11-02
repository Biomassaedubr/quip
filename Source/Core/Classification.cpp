#include "Classification.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "Selection.hpp"

namespace quip {
  namespace {
    bool isWordCharacter (char character) {
      return std::isalnum(character) || character == '_';
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
    --extent;
    
    return Selection(origin.location(), extent.location());
  }
  
  Selection selectNextWord (const Document & document, const Selection & basis) {
    // Move until a non-word character.
    DocumentIterator cursor = document.at(basis.extent());
    while (cursor != document.end() && isWordCharacter(*cursor)) {
      ++cursor;
    }
    
    // Then move until a word character.
    while (cursor != document.end() && !isWordCharacter(*cursor)) {
      ++cursor;
    }
    
    // Save the origin, and move to the next non-word character...
    Location origin = cursor.location();
    while (cursor != document.end() && isWordCharacter(*cursor)) {
      ++cursor;
    }
    
    // ...but don't actually include that non-word character.
    if (cursor != document.end()) {
      --cursor;
    }
    
    return Selection(origin, cursor.location());
  }
  
  Selection selectPriorWord (const Document & document, const Selection & basis) {
    // Move until a non-word character.
    DocumentIterator cursor = document.at(basis.extent());
    while (cursor != document.begin() && isWordCharacter(*cursor)) {
      --cursor;
    }
    
    // Then move until a word character.
    while (cursor != document.begin() && !isWordCharacter(*cursor)) {
      --cursor;
    }
    
    // Save the extent, and move to the next non-word character...
    Location extent = cursor.location();
    while (cursor != document.begin() && isWordCharacter(*cursor)) {
      --cursor;
    }
    
    // ...but don't actually include that non-word character.
    if (cursor != document.begin()) {
      ++cursor;
    }
    
    return Selection(cursor.location(), extent);
  }
  
  Selection selectThisLine (const Document & document, const Selection & basis) {
    Location origin(0, basis.origin().row());
    
    std::uint64_t row = basis.extent().row();
    Location extent(document.row(row).size() - 1, row);
    return Selection(origin, extent);
  }
}
