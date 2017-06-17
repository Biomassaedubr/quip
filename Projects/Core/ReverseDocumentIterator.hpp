#pragma once

#include "DocumentIterator.hpp"
#include "Optional.hpp"

#include <cstddef>
#include <iterator>

namespace quip {
  // A reverse iterator for a document, providing character-by-character traversal of the content
  // of a document in reverse document order.
  //
  // Reverse document iterators only provide read-only access to the underlying document.
  struct ReverseDocumentIterator {
    typedef std::int64_t difference_type;
    typedef const char value_type;
    typedef const char* pointer;
    typedef const char& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
    
    explicit ReverseDocumentIterator(const DocumentIterator& basis);
    
    const Document& document() const;
    Location location() const;
    
    bool isBegin() const;
    bool isEnd() const;
    
    char operator*() const;
    
    ReverseDocumentIterator& operator++();
    ReverseDocumentIterator operator++(int);
    ReverseDocumentIterator& operator--();
    ReverseDocumentIterator operator--(int);

    bool operator==(const ReverseDocumentIterator& other) const;
    bool operator!=(const ReverseDocumentIterator& other) const;
    
  private:
    DocumentIterator m_basis;
  };
}
