#include "DocumentIterator.hpp"

#include "Document.hpp"

namespace quip {
  DocumentIterator::DocumentIterator (const Document & document, Location location)
  : m_document(&document)
  , m_location(location) {
  }
  
  Location DocumentIterator::location () const {
    return m_location;
  }
  
  char DocumentIterator::operator* () const {
    return m_document->row(m_location.row())[m_location.column()];
  }
  
  DocumentIterator & DocumentIterator::operator++ () {
    bool isOnLastColumn = m_location.column() == m_document->row(m_location.row()).length() - 1;
    bool isOnLastRow = m_location.row() == m_document->rows() - 1;
    if (isOnLastColumn && !isOnLastRow) {
      m_location = Location(0, m_location.row() + 1);
    } else {
      m_location = m_location.adjustBy(1, 0);
    }
    
    return *this;
  }
  
  DocumentIterator & DocumentIterator::operator-- () {
    if (m_location.column() == 0) {
      std::size_t row = m_location.row() - 1;
      m_location = Location(m_document->row(row).size() - 1, row);
    } else {
      m_location = m_location.adjustBy(-1, 0);
    }
    
    return *this;
  }
  
  bool operator== (const DocumentIterator & left, const DocumentIterator & right) {
    return left.m_document == right.m_document && left.m_location == right.m_location;
  }
  
  bool operator!= (const DocumentIterator & left, const DocumentIterator & right) {
    return !(left == right);
  }
}
