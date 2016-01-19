#pragma once

#include "Location.hpp"

#include <cstddef>
#include <iterator>

namespace quip {
  struct Document;
  
  struct DocumentIterator {
    DocumentIterator ();
    DocumentIterator (const Document & document, Location location);
    
    Location location () const;
    
    char operator* () const;
    
    DocumentIterator & operator++ ();
    DocumentIterator & operator-- ();
    
    friend bool operator== (const DocumentIterator & left, const DocumentIterator & right);
    friend bool operator!= (const DocumentIterator & left, const DocumentIterator & right);
    
  private:
    const Document * m_document;
    Location m_location;
  };
}

namespace std {
  template<>
  struct iterator_traits<quip::DocumentIterator> {
    typedef char value_type;
    typedef std::ptrdiff_t difference_type;
    typedef const char * pointer;
    typedef const char & reference;
    typedef std::bidirectional_iterator_tag iterator_category;
  };
}