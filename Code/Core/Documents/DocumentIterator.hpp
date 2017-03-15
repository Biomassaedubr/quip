#pragma once

#include "Location.hpp"

#include <cstddef>
#include <iterator>

namespace quip {
  struct Document;
  
  struct DocumentIterator {
    DocumentIterator(const Document& document, Location location);
    
    Location location() const;
    
    char operator*() const;
    
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
    
    friend bool operator== (const DocumentIterator& left, const DocumentIterator& right);
    friend bool operator!= (const DocumentIterator& left, const DocumentIterator& right);
    
  private:
    const Document* m_document;
    Location m_location;
  };
}

namespace std {
  template<>
  struct iterator_traits<quip::DocumentIterator> {
    typedef char value_type;
    typedef std::ptrdiff_t difference_type;
    typedef const char* pointer;
    typedef const char& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
  };
}

#include "DocumentIterator.inl"
