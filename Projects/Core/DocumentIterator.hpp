#pragma once

#include "Location.hpp"

#include <cstddef>
#include <iterator>

#include <functional>

namespace quip {
  struct Document;
  
  // An iterator for a document, providing character-by-character traversal of the content
  // of a document in document order.
  //
  // Document iterators only provide read-only access to the underlying document.
  struct DocumentIterator {
    typedef std::int64_t difference_type;
    typedef const char value_type;
    typedef const char* pointer;
    typedef const char& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
    
    DocumentIterator(const Document& document, const Location& location);
    
    Location location() const;
    
    char operator*() const;
    
    DocumentIterator& operator++();
    DocumentIterator operator++(int);
    DocumentIterator& operator--();
    DocumentIterator operator--(int);

    bool operator==(const DocumentIterator& other) const;
    bool operator!=(const DocumentIterator& other) const;
    
  private:
    const Document* m_document;
    Location m_location;    
  };
}
