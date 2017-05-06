#include "Selector.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "Selection.hpp"

namespace quip {
  namespace {
    bool isStartItemCharacter(char character) {
      return character == ',' || character == '(';
    }
    
    bool isNotStartItemCharacter(char character) {
      return !isStartItemCharacter(character);
    }
    
    bool isEndItemCharacter(char character) {
      return character == ',' || character == ')';
    }
    
    bool isNotEndItemCharacter(char character) {
      return !isEndItemCharacter(character);
    }
    
    bool isOpenBlockCharacter(char character) {
      return character == '{' || character == '[' || character == '<';
    }
    
    bool isNotOpenBlockCharacter(char character) {
      return !isOpenBlockCharacter(character);
    }
    
    bool isCloseBlockCharacter(char character) {
      return character == '}' || character == ']' || character == '>';
    }
    
    bool isNotCloseBlockCharacter(char character) {
      return !isCloseBlockCharacter(character);
    }
    
    bool isWordCharacter(char character) {
      return std::isalnum(character);
    }
    
    bool isNotWordCharacter(char character) {
      return !isWordCharacter(character);
    }
    
    bool isWhitespaceExceptNewline(char character) {
      return std::isspace(character) && character != '\n';
    }
    
    Document::ConstIterator selectTrailingWhitespaceIfApplicable(const Document& document, Document::ConstIterator& start) {
      Document::ConstIterator result = start;
      if (result != document.end()) {
        // Try to select any (appropriate) trailing whitespace as well.
        ++result;
        if (isWhitespaceExceptNewline(*result)) {
          result.advanceWhile(isWhitespaceExceptNewline);
        } else {
          --result;
        }
      }
      
      return result;
    }
  }
  
  Optional<Selection> selectWord(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    Document::ConstIterator origin = document.from(basis.origin());
    origin.reverseWhile(isWordCharacter);
    
    Document::ConstIterator extent = document.from(basis.extent());
    extent.advanceWhile(isWordCharacter);
    extent = selectTrailingWhitespaceIfApplicable(document, extent);
    
    // If the selection didn't change, the basis was already a full word selection. In this case,
    // the next full word should be selected.
    if(basis.origin() == origin.location() && basis.extent() == extent.location() && extent != document.end()) {
      origin = ++extent;
      if (origin == document.end()) {
        // Reached the end of the document.
        return Optional<Selection>(basis);
      }
      
      extent.advanceWhile(isWordCharacter);
      extent = selectTrailingWhitespaceIfApplicable(document, extent);
    }
    
    return Optional<Selection>(Selection(origin.location(), extent.location()));
  }
  
  Optional<Selection> selectPriorWord(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    Document::ConstIterator origin = document.from(basis.origin());
    origin.reverseUntil(isNotWordCharacter);
    if (origin == document.begin()) {
      return Optional<Selection>();
    }
    
    origin.reverseUntil(isWordCharacter);
    if (origin == document.begin()) {
      return Optional<Selection>();
    }
    
    return Optional<Selection>(selectWord(document, Selection(origin.location())));
  }

  Optional<Selection> selectRemainingWord(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    Document::ConstIterator extent = document.from(basis.extent());
    extent.advanceWhile(isWordCharacter);
    extent = selectTrailingWhitespaceIfApplicable(document, extent);
    
    return Optional<Selection>(Selection(basis.origin(), extent.location()));
  }
  
  Optional<Selection> selectThisLine(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    Location origin(0, basis.origin().row());
    
    std::uint64_t row = basis.extent().row();
    Location extent(document.row(row).size() - 1, row);
    return Optional<Selection>(Selection(origin, extent));
  }
  
  Optional<Selection> selectNextLine(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    std::uint64_t row = basis.extent().row();
    if (row + 1 < document.rows()) {
      ++row;
      Location origin(0, row);
      Location extent(document.row(row).size() - 1, row);
      return Optional<Selection>(Selection(origin, extent));
    }
    
    return Optional<Selection>(selectThisLine(document, basis));
  }
  
  Optional<Selection> selectPriorLine(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    std::uint64_t row = basis.origin().row();
    if (row > 0) {
      --row;
      Location origin(0, row);
      Location extent(document.row(row).size() - 1, row);
      return Optional<Selection>(Selection(origin, extent));
    }
    
    return Optional<Selection>(selectThisLine(document, basis));
  }
  
  Optional<Selection> selectBlocks(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    Document::ConstIterator origin = document.from(basis.origin());
    origin.reverseWhile(isNotOpenBlockCharacter);
    
    Document::ConstIterator extent = document.from(basis.extent());
    extent.advanceWhile(isNotCloseBlockCharacter);
    
    if(basis.origin() == origin.location() && basis.extent() == extent.location() && origin != document.begin() && extent != document.end()) {
      --origin;
      ++extent;
      return selectBlocks(document, Selection(origin.location(), extent.location()));
    }
    
    return Optional<Selection>(Selection(origin.location(), extent.location()));
  }
  
  Optional<Selection> selectItem(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    Document::ConstIterator origin = document.from(basis.origin());
    origin.reverseWhile(isNotStartItemCharacter);
    
    Document::ConstIterator extent = document.from(basis.extent());
    extent.advanceWhile(isNotEndItemCharacter);
    extent = selectTrailingWhitespaceIfApplicable(document, extent);
    
    // If the selection didn't change, the basis was already a full item selection. In this case,
    // the next full item should be selected.
    if(basis.origin() == origin.location() && basis.extent() == extent.location() && extent != document.end()) {
      extent.advanceUntil(isStartItemCharacter);
      origin = ++extent;
      extent.advanceWhile(isNotEndItemCharacter);
      extent = selectTrailingWhitespaceIfApplicable(document, extent);
    }
    
    return Optional<Selection>(Selection(origin.location(), extent.location()));
  }
}
