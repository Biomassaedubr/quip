#include "Selector.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "ReverseDocumentIterator.hpp"
#include "Selection.hpp"
#include "Traversal.hpp"

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
    
    bool isWhitespaceExceptNewline(char character) {
      return std::isspace(character) && character != '\n';
    }
    
    template<typename IteratorType>
    IteratorType selectTrailingWhitespaceIfApplicable(IteratorType& start) {
      IteratorType result = start;
      if (!result.isEnd()) {
        // Try to select any (appropriate) trailing whitespace as well.
        result = std::next(result);
        if (isWhitespaceExceptNewline(*result)) {
          result = Traversal::advanceWhile(result, isWhitespaceExceptNewline);
        } else {
          result = std::prev(result);
        }
      }
      
      return result;
    }
    
    template<typename IteratorType>
    Optional<Selection> selectWord(const Selection& basis, IteratorType origin, IteratorType extent) {
      origin = Traversal::retreatWhile(origin, isWordCharacter);
      extent = Traversal::advanceWhile(extent, isWordCharacter);
      extent = selectTrailingWhitespaceIfApplicable(extent);
      
      // If the selection didn't change, the basis was already a full word selection. In this case,
      // the next full word should be selected.
      Selection result(origin, extent);
      if (result == basis && !extent.isEnd()) {
        origin = std::next(extent);
        if (origin.isEnd()) {
          // Reached the end of the document.
          return Optional<Selection>(basis);
        }
        
        extent = Traversal::advanceWhile(origin, isWordCharacter);
        extent = selectTrailingWhitespaceIfApplicable(extent);
        return Optional<Selection>(Selection(origin.location(), extent.location()));
      }
      
      return result;
    }
  }
  
  Optional<Selection> selectThisOrNextWord(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    DocumentIterator head = document.at(basis.origin());
    DocumentIterator tail = document.at(basis.extent());
    return selectWord(basis, head, tail);
  }
  
  Optional<Selection> selectPriorWord(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    ReverseDocumentIterator head = document.rat(basis.extent());
    ReverseDocumentIterator tail = document.rat(basis.origin());
    return selectWord(basis, head, tail);
  }

  Optional<Selection> selectRemainingWord(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    DocumentIterator extent = document.at(basis.extent());
    extent = Traversal::advanceWhile(extent, isWordCharacter);
    extent = selectTrailingWhitespaceIfApplicable(extent);
    
    return Optional<Selection>(Selection(basis.origin(), extent.location()));
  }
  
  Optional<Selection> selectThisOrNextLine(const Document& document, const Selection& basis) {
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
    
    return Optional<Selection>(selectThisOrNextLine(document, basis));
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
    
    return Optional<Selection>(selectThisOrNextLine(document, basis));
  }
  
  Optional<Selection> selectBlocks(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    DocumentIterator origin = document.at(basis.origin());
    origin = Traversal::retreatWhile(origin, isNotOpenBlockCharacter);
    
    DocumentIterator extent = document.at(basis.extent());
    extent = Traversal::advanceWhile(extent, isNotCloseBlockCharacter);
    
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
    
    DocumentIterator origin = document.at(basis.origin());
    origin = Traversal::retreatWhile(origin, isNotStartItemCharacter);
    
    DocumentIterator extent = document.at(basis.extent());
    extent = Traversal::advanceWhile(extent, isNotEndItemCharacter);
    extent = selectTrailingWhitespaceIfApplicable(extent);
    
    // If the selection didn't change, the basis was already a full item selection. In this case,
    // the next full item should be selected.
    if(basis.origin() == origin.location() && basis.extent() == extent.location() && extent != document.end()) {
      extent = Traversal::advanceUntil(extent, isStartItemCharacter);
      extent = std::next(extent);
      origin = extent;
      extent = Traversal::advanceWhile(extent, isNotEndItemCharacter);
      extent = selectTrailingWhitespaceIfApplicable(extent);
    }
    
    return Optional<Selection>(Selection(origin.location(), extent.location()));
  }
}
