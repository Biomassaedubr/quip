#include "Document.hpp"

namespace quip {
  template<typename ElementType, bool IsConst>
  DocumentIterator<ElementType, IsConst>::DocumentIterator(DocumentIterator<ElementType, IsConst>::DocumentType& document, const Location& location)
  : m_document(&document)
  , m_location(location) {
  }
  
  template<typename ElementType, bool IsConst>
  const Location& DocumentIterator<ElementType, IsConst>::location() const {
    return m_location;
  }
  
  template<typename ElementType, bool IsConst>
  typename DocumentIterator<ElementType, IsConst>::ValueType DocumentIterator<ElementType, IsConst>::operator*() const {
    return m_document->row(m_location.row())[m_location.column()];
  }
  
  template<typename ElementType, bool IsConst>
  DocumentIterator<ElementType, IsConst>& DocumentIterator<ElementType, IsConst>::operator++() {
    bool isOnLastColumn = m_location.column() == m_document->row(m_location.row()).length() - 1;
    bool isOnLastRow = m_location.row() == m_document->rows() - 1;
    if (isOnLastColumn && !isOnLastRow) {
      m_location = Location(0, m_location.row() + 1);
    } else {
      m_location = m_location.adjustBy(1, 0);
    }
    
    return *this;
  }
  
  template<typename ElementType, bool IsConst>
  DocumentIterator<ElementType, IsConst>& DocumentIterator<ElementType, IsConst>::operator--() {
    if (m_location.column() == 0) {
      std::size_t row = m_location.row() - 1;
      m_location = Location(m_document->row(row).size() - 1, row);
    } else {
      m_location = m_location.adjustBy(-1, 0);
    }
    
    return *this;
  }
  
  template<typename ElementType, bool IsConst>
  DocumentIterator<ElementType, IsConst>& DocumentIterator<ElementType, IsConst>::advanceByRows(std::uint64_t rows) {
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
  
  template<typename ElementType, bool IsConst>
  DocumentIterator<ElementType, IsConst>& DocumentIterator<ElementType, IsConst>::reverseByRows(std::uint64_t rows) {
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
  
  template<typename ElementType, bool IsConst>
  template<typename PredicateType>
  DocumentIterator<ElementType, IsConst>& DocumentIterator<ElementType, IsConst>::advanceWhile(PredicateType predicate) {
    DocumentIterator<ElementType, IsConst>& self = *this;
    bool pass = predicate(*self);
    if (pass) {
      while (self != m_document->end() && pass) {
        ++self;
        pass = predicate(*self);
      }
      
      if (!pass) {
        --self;
      }
    }
    
    return self;
  }
  
  template<typename ElementType, bool IsConst>
  template<typename PredicateType>
  DocumentIterator<ElementType, IsConst>& DocumentIterator<ElementType, IsConst>::advanceUntil(PredicateType predicate) {
    DocumentIterator& self = *this;
    while(self != m_document->end() && !predicate(*self)) {
      ++self;
    }
    
    return self;
  }
  
  template<typename ElementType, bool IsConst>
  template<typename PredicateType>
  DocumentIterator<ElementType, IsConst>& DocumentIterator<ElementType, IsConst>::reverseWhile(PredicateType predicate) {
    DocumentIterator<ElementType, IsConst>& self = *this;
    bool pass = predicate(*self);
    if (pass) {
      while (self != m_document->begin() && pass) {
        --self;
        pass = predicate(*self);
      }
      
      if (!pass) {
        ++self;
      }
    }
    
    return self;
  }
  
  template<typename ElementType, bool IsConst>
  template<typename PredicateType>
  DocumentIterator<ElementType, IsConst>& DocumentIterator<ElementType, IsConst>::reverseUntil(PredicateType predicate) {
    DocumentIterator<ElementType, IsConst>& self = *this;
    while(self != m_document->begin() && !predicate(*self)) {
      --self;
    }
    
    return self;
  }
  
  template<typename ElementType, bool IsConst>
  bool DocumentIterator<ElementType, IsConst>::operator==(const DocumentIterator<ElementType, IsConst>& other) {
    return m_document == other.m_document && m_location == other.m_location;
  }
  
  template<typename ElementType, bool IsConst>
  bool DocumentIterator<ElementType, IsConst>::operator!=(const DocumentIterator<ElementType, IsConst>& other) {
    return !(*this == other);
  }
  
  template<typename IteratorType>
  ReverseDocumentIterator<IteratorType>::ReverseDocumentIterator(const IteratorType& iterator)
  : m_iterator(iterator) {
  }
  
  template<typename IteratorType>
  const Location& ReverseDocumentIterator<IteratorType>::location() const {
    return m_iterator.location();
  }
  
  template<typename IteratorType>
  typename IteratorType::ValueType ReverseDocumentIterator<IteratorType>::operator*() const {
    IteratorType actual = m_iterator;
    --actual;
    return *actual;
  }
  
  template<typename IteratorType>
  ReverseDocumentIterator<IteratorType>& ReverseDocumentIterator<IteratorType>::operator++() {
    --m_iterator;
    return *this;
  }
  
  template<typename IteratorType>
  ReverseDocumentIterator<IteratorType>& ReverseDocumentIterator<IteratorType>::operator--() {
    ++m_iterator;
    return *this;
  }

  template<typename IteratorType>
  bool ReverseDocumentIterator<IteratorType>::operator==(const ReverseDocumentIterator<IteratorType>& other) {
    return m_iterator == other.m_iterator;
  }
  
  template<typename IteratorType>
  bool ReverseDocumentIterator<IteratorType>::operator!=(const ReverseDocumentIterator<IteratorType>& other) {
    return !(*this == other);
  }
}
