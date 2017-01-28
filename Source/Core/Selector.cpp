#include "Selector.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "Selection.hpp"

namespace quip {
  namespace {
    bool isWordCharacter(char character) {
      return std::isalnum(character);
    }
    
    bool isNotWordCharacter(char character) {
      return !isWordCharacter(character);
    }
    
    bool isWhitespaceExceptNewline(char character) {
      return std::isspace(character) && character != '\n';
    }
  }
  
  Selection selectThisWord(const Document& document, const Selection& basis) {
    DocumentIterator origin = document.at(basis.origin());
    origin.reverseWhile(isWordCharacter);
    
    DocumentIterator extent = origin;
    extent.advanceWhile(isWordCharacter);
    if (extent != document.end()) {
      // Try to select any (appropriate) trailing whitespace as well.
      ++extent;
      if (isWhitespaceExceptNewline(*extent)) {
        extent.advanceWhile(isWhitespaceExceptNewline);
      } else {
        --extent;
      }
    }
   
    return Selection(origin.location(), extent.location());
  }
  
  Selection selectNextWord(const Document& document, const Selection& basis) {
    DocumentIterator origin = document.at(basis.extent());
    origin.advanceUntil(isNotWordCharacter);
    if (origin == document.end()) {
      return basis;
    }
    
    origin.advanceUntil(isWordCharacter);
    if (origin == document.end()) {
      return basis;
    }
    
    return selectThisWord(document, Selection(origin.location()));
  }
  
  Selection selectPriorWord(const Document& document, const Selection& basis) {
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

  Selection selectRemainingWord(const Document& document, const Selection& basis) {
    DocumentIterator extent = document.at(basis.extent());
    extent.advanceWhile(isWordCharacter);
    if (extent != document.end()) {
      // Try to select any (appropriate) trailing whitespace as well.
      ++extent;
      if (isWhitespaceExceptNewline(*extent)) {
        extent.advanceWhile(isWhitespaceExceptNewline);
      } else {
        --extent;
      }
    }
    
    return Selection(basis.origin(), extent.location());
  }
  
  Selection selectThisLine(const Document& document, const Selection& basis) {
    Location origin(0, basis.origin().row());
    
    std::uint64_t row = basis.extent().row();
    Location extent(document.row(row).size() - 1, row);
    return Selection(origin, extent);
  }
  
  Selection selectNextLine(const Document& document, const Selection& basis) {
    std::uint64_t row = basis.extent().row();
    if (row + 1 < document.rows()) {
      ++row;
      Location origin(0, row);
      Location extent(document.row(row).size() - 1, row);
      return Selection(origin, extent);
    }
    
    return selectThisLine(document, basis);
  }
  
  Selection selectPriorLine(const Document& document, const Selection& basis) {
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
