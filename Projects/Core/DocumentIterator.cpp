#include "DocumentIterator.hpp"

namespace quip {
  DocumentIterator::DocumentIterator(const Document& document, const Location& location)
  : m_document(&document)
  , m_location(location) {
  }
  
  const Location& DocumentIterator::location() const {
    return m_location;
  }
  
  char DocumentIterator::operator*() const {
    return m_document->row(m_location.row())[m_location.column()];
  }
  
  DocumentIterator& DocumentIterator::operator++() {
    bool isOnLastColumn = m_location.column() == m_document->row(m_location.row()).length() - 1;
    bool isOnLastRow = m_location.row() == m_document->rows() - 1;
    if (isOnLastColumn && !isOnLastRow) {
      m_location = Location(0, m_location.row() + 1);
    } else {
      m_location = m_location.adjustBy(1, 0);
    }
    
    return *this;
  }
  
  DocumentIterator DocumentIterator::operator++(int) {
    DocumentIterator result = *this;
    ++result;
    return result;
  }
  
  DocumentIterator& DocumentIterator::operator--() {
    if (m_location.column() == 0) {
      std::size_t row = m_location.row() - 1;
      m_location = Location(m_document->row(row).size() - 1, row);
    } else {
      m_location = m_location.adjustBy(-1, 0);
    }
    
    return *this;
  }
  
  DocumentIterator DocumentIterator::operator--(int) {
    DocumentIterator result = *this;
    --result;
    return result;
  }
  
  DocumentIterator& DocumentIterator::advanceByRows(std::uint64_t rows) {
    if (m_location.row() == m_document->rows() - 1) {
      // Attempting to move by a full row while on the last row should advance
      // the iterator to the end of the document.
      m_location = m_document->end().location();
    } else {
      std::uint64_t targetRow = m_location.row() + rows;
      std::uint64_t columnsInTargetRow = m_document->row(targetRow).size();
      
      m_location = Location(std::min(columnsInTargetRow - 1, m_location.column()), targetRow);
    }
    
    return *this;
  }
  
  DocumentIterator& DocumentIterator::reverseByRows(std::uint64_t rows) {
    if (m_location.row() == 0) {
      // Attempting to move by a full row while on the first row should advance
      // the iterator to the start of the document.
      m_location = m_document->begin().location();
    } else {
      std::uint64_t targetRow = m_location.row() - rows;
      std::uint64_t columnsInTargetRow = m_document->row(targetRow).size();
      
      m_location = Location(std::min(columnsInTargetRow - 1, m_location.column()), targetRow);
    }
    
    return *this;
  }
  
  bool DocumentIterator::operator==(const DocumentIterator& other) {
    return m_document == other.m_document && m_location == other.m_location;
  }
  
  bool DocumentIterator::operator!=(const DocumentIterator& other) {
    return !(*this == other);
  }
}
