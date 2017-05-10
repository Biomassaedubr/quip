#pragma once

#include "DocumentIterator.hpp"

namespace quip {
  struct ReverseDocumentIterator {
    ReverseDocumentIterator(const DocumentIterator& underlying);
    
    const Location& location() const;
    
    char operator*() const;
    
    ReverseDocumentIterator& operator++();
    ReverseDocumentIterator& operator--();
    
    ReverseDocumentIterator& advanceByRows(std::uint64_t rows);
    ReverseDocumentIterator& reverseByRows(std::uint64_t rows);
    
    template<typename PredicateType>
    ReverseDocumentIterator& advanceWhile(PredicateType predicate);
    
    template<typename PredicateType>
    ReverseDocumentIterator& advanceUntil(PredicateType predicate);
    
    template<typename PredicateType>
    ReverseDocumentIterator& reverseWhile(PredicateType predicate);
    
    template<typename PredicateType>
    ReverseDocumentIterator& reverseUntil(PredicateType predicate);
    
    bool operator==(const ReverseDocumentIterator& other);
    bool operator!=(const ReverseDocumentIterator& other);
    
  private:
    DocumentIterator m_iterator;
  };
}
