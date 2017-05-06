#pragma once

#include "Location.hpp"

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace quip {
  struct Document;
  
  template<typename ElementType, bool IsConst>
  struct DocumentIterator {
    typedef ElementType value_type;
    typedef std::ptrdiff_t difference_type;
    typedef const ElementType* pointer;
    typedef const ElementType& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
    
    typedef typename std::conditional<IsConst, const Document, Document>::type DocumentType;
    
    DocumentIterator(DocumentType& document, const Location& location);
    
    const Location& location() const;
    
    ElementType operator*() const;
    
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
}

#include "DocumentIterator.inl"
