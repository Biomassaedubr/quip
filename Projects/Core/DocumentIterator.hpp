#pragma once

#include "Location.hpp"

#include <cstddef>
#include <type_traits>

namespace quip {
  struct Document;
  
  template<typename ElementType, bool IsConst>
  struct DocumentIterator {
    typedef ElementType ValueType;
    typedef typename std::conditional<IsConst, const Document, Document>::type DocumentType;
    
    DocumentIterator(DocumentType& document, const Location& location);
    
    const Location& location() const;
    
    ValueType operator*() const;
    
    DocumentIterator& operator++();
    DocumentIterator& operator--();
    
    DocumentIterator& advanceByRows(std::uint64_t rows);
    DocumentIterator& reverseByRows(std::uint64_t rows);
    
    // Advance the iterator while a predicate passes for the character referred to by the iterator.
    //
    // The iterator is not updated if the predicate fails for the character initially referred to.
    // Otherwise, the iterator will refer to the last character that passed the predicate.
    template<typename PredicateType>
    DocumentIterator& advanceWhile(PredicateType predicate);
    
    // Advance the iterator until a predicate passes for the character referred to by the iterator.
    //
    // The iterator is not updated if the predicate passes for the character initially referred to.
    // Otherwise, the iterator will refer to the first character that passed the predicate.
    template<typename PredicateType>
    DocumentIterator& advanceUntil(PredicateType predicate);
    
    // Reverse the iterator while a predicate passes for the character referred to by the iterator.
    //
    // The iterator is not updated if the predicate fails for the character initially referred to.
    // Otherwise, the iterator will refer to the last character that passed the predicate.
    template<typename PredicateType>
    DocumentIterator& reverseWhile(PredicateType predicate);
    
    // Reverse the iterator until a predicate passes for the character referred to by the iterator.
    //
    // The iterator is not updated if the predicate passes for the character initially referred to.
    // Otherwise, the iterator will refer to the first character that passed the predicate.
    template<typename PredicateType>
    DocumentIterator& reverseUntil(PredicateType predicate);
    
    bool operator==(const DocumentIterator& other);
    bool operator!=(const DocumentIterator& other);
    
  private:
    DocumentType* m_document;
    Location m_location;
  };
  
  template<typename IteratorType>
  struct ReverseDocumentIterator {
    ReverseDocumentIterator(const IteratorType& underlying);
    
    const Location& location() const;
    
    typename IteratorType::ValueType operator*() const;
    
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
    IteratorType m_iterator;
  };
}

#include "DocumentIterator.inl"
