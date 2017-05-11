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
    
    bool isWhitespaceExceptNewline(char character) {
      return std::isspace(character) && character != '\n';
    }
    
    DocumentIterator selectTrailingWhitespaceIfApplicable(DocumentIterator& start, const Traversal& traversal) {
      DocumentIterator result = start;
      if (result != traversal.advanceTo()) {
        // Try to select any (appropriate) trailing whitespace as well.
        result = traversal.advance(result);
        if (isWhitespaceExceptNewline(*result)) {
          result = traversal.advanceWhile(result, isWhitespaceExceptNewline);
        } else {
          result = traversal.retreat(result);
        }
      }
      
      return result;
    }
    
    Optional<Selection> selectWord(const Document& document, const Selection& basis, DocumentIterator origin, DocumentIterator extent, const Traversal& traversal) {
      if (document.isEmpty()) {
        return Optional<Selection>();
      }
      
      DocumentIterator basisOrigin = origin;
      DocumentIterator basisExtent = extent;
      origin = traversal.retreatWhile(origin, isWordCharacter);
      extent = traversal.advanceWhile(extent, isWordCharacter);
      extent = selectTrailingWhitespaceIfApplicable(extent, traversal);
      
      // If the selection didn't change, the basis was already a full word selection. In this case,
      // the next full word should be selected.
      if(basisOrigin.location() == origin.location() && basisExtent.location() == extent.location() && extent != traversal.advanceTo()) {
        extent = traversal.advance(extent);
        origin = extent;
        if (origin == traversal.advanceTo()) {
          // Reached the end of the document.
          return Optional<Selection>(basis);
        }
        
        extent = traversal.advanceWhile(extent, isWordCharacter);
        extent = selectTrailingWhitespaceIfApplicable(extent, traversal);
      }
      
      return Optional<Selection>(Selection(origin.location(), extent.location()));

    }
  }
  
  Optional<Selection> selectWord(const Document& document, const Selection& basis) {
    DocumentIterator head = document.from(basis.origin());
    DocumentIterator tail = document.from(basis.extent());
    return selectWord(document, basis, head, tail, Traversal::documentOrder(document));
  }
  
  Optional<Selection> selectPriorWord(const Document& document, const Selection& basis) {
    DocumentIterator head = document.from(basis.extent());
    DocumentIterator tail = document.from(basis.origin());
    return selectWord(document, basis, head, tail, Traversal::reverseDocumentOrder(document));
  }

  Optional<Selection> selectRemainingWord(const Document& document, const Selection& basis) {
    if (document.isEmpty()) {
      return Optional<Selection>();
    }
    
    DocumentIterator extent = document.from(basis.extent());
    extent.advanceWhile(isWordCharacter);
    extent = selectTrailingWhitespaceIfApplicable(extent, Traversal::documentOrder(document));
    
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
    
    DocumentIterator origin = document.from(basis.origin());
    origin.reverseWhile(isNotOpenBlockCharacter);
    
    DocumentIterator extent = document.from(basis.extent());
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
    
    DocumentIterator origin = document.from(basis.origin());
    origin.reverseWhile(isNotStartItemCharacter);
    
    DocumentIterator extent = document.from(basis.extent());
    extent.advanceWhile(isNotEndItemCharacter);
    extent = selectTrailingWhitespaceIfApplicable(extent, Traversal::documentOrder(document));
    
    // If the selection didn't change, the basis was already a full item selection. In this case,
    // the next full item should be selected.
    if(basis.origin() == origin.location() && basis.extent() == extent.location() && extent != document.end()) {
      extent.advanceUntil(isStartItemCharacter);
      origin = ++extent;
      extent.advanceWhile(isNotEndItemCharacter);
      extent = selectTrailingWhitespaceIfApplicable(extent, Traversal::documentOrder(document));
    }
    
    return Optional<Selection>(Selection(origin.location(), extent.location()));
  }
}
