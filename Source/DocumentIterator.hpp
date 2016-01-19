#pragma once

#include "Location.hpp"

#include <cstddef>

namespace quip {
  struct Document;
  
  struct DocumentIterator {
    DocumentIterator ();
    DocumentIterator (Document & document, Location location);
    
    char operator* () const;
    
    DocumentIterator & operator++ ();
    DocumentIterator & operator-- ();
    
    friend bool operator== (const DocumentIterator & left, const DocumentIterator & right);
    friend bool operator!= (const DocumentIterator & left, const DocumentIterator & right);
    
  private:
    Document * m_document;
    Location m_location;
  };
}