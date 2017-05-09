#include "Document.hpp"

namespace quip {
  template<bool IsConst>
  DocumentIterator<IsConst>::DocumentIterator(DocumentIterator<IsConst>::DocumentType& document, const Location& location)
  : m_document(&document)
  , m_location(location) {
  }
  
  template<bool IsConst>
  const Location& DocumentIterator<IsConst>::location() const {
    return m_location;
  }
  
  template<bool IsConst>
  char DocumentIterator<IsConst>::operator*() const {
    return m_document->row(m_location.row())[m_location.column()];
  }
  
  template<bool IsConst>
  DocumentIterator<IsConst>& DocumentIterator<IsConst>::operator++() {
    bool isOnLastColumn = m_location.column() == m_document->row(m_location.row()).length() - 1;
    bool isOnLastRow = m_location.row() == m_document->rows() - 1;
    if (isOnLastColumn && !isOnLastRow) {
      m_location = Location(0, m_location.row() + 1);
    } else {
      m_location = m_location.adjustBy(1, 0);
    }
    
    return *this;
  }
  
  template<bool IsConst>
  DocumentIterator<IsConst> DocumentIterator<IsConst>::operator++(int) {
    DocumentIterator<IsConst> result = *this;
    ++result;
    return result;
  }
  
  template<bool IsConst>
  DocumentIterator<IsConst>& DocumentIterator<IsConst>::operator--() {
    if (m_location.column() == 0) {
      std::size_t row = m_location.row() - 1;
      m_location = Location(m_document->row(row).size() - 1, row);
    } else {
      m_location = m_location.adjustBy(-1, 0);
    }
    
    return *this;
  }
  
  template<bool IsConst>
  DocumentIterator<IsConst> DocumentIterator<IsConst>::operator--(int) {
    DocumentIterator<IsConst> result = *this;
    --result;
    return result;
  }
  
  template<bool IsConst>
  DocumentIterator<IsConst>& DocumentIterator<IsConst>::advanceByRows(std::uint64_t rows) {
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
  
  template<bool IsConst>
  DocumentIterator<IsConst>& DocumentIterator<IsConst>::reverseByRows(std::uint64_t rows) {
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
  
  template<bool IsConst>
  template<typename PredicateType>
  DocumentIterator<IsConst>& DocumentIterator<IsConst>::advanceWhile(PredicateType predicate) {
    DocumentIterator<IsConst>& self = *this;
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
  
  template<bool IsConst>
  template<typename PredicateType>
  DocumentIterator<IsConst>& DocumentIterator<IsConst>::advanceUntil(PredicateType predicate) {
    DocumentIterator& self = *this;
    while(self != m_document->end() && !predicate(*self)) {
      ++self;
    }
    
    return self;
  }
  
  template<bool IsConst>
  template<typename PredicateType>
  DocumentIterator<IsConst>& DocumentIterator<IsConst>::reverseWhile(PredicateType predicate) {
    DocumentIterator<IsConst>& self = *this;
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
  
  template<bool IsConst>
  template<typename PredicateType>
  DocumentIterator<IsConst>& DocumentIterator<IsConst>::reverseUntil(PredicateType predicate) {
    DocumentIterator<IsConst>& self = *this;
    while(self != m_document->begin() && !predicate(*self)) {
      --self;
    }
    
    return self;
  }
  
  template<bool IsConst>
  bool DocumentIterator<IsConst>::operator==(const DocumentIterator<IsConst>& other) {
    return m_document == other.m_document && m_location == other.m_location;
  }
  
  template<bool IsConst>
  bool DocumentIterator<IsConst>::operator!=(const DocumentIterator<IsConst>& other) {
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
  char ReverseDocumentIterator<IteratorType>::operator*() const {
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
  template<typename PredicateType>
  ReverseDocumentIterator<IteratorType>& ReverseDocumentIterator<IteratorType>::advanceWhile(PredicateType predicate) {
    m_iterator.reverseWhile(predicate);
    return *this;
  }
  
  template<typename IteratorType>
  template<typename PredicateType>
  ReverseDocumentIterator<IteratorType>& ReverseDocumentIterator<IteratorType>::reverseWhile(PredicateType predicate) {
    m_iterator.advanceWhile(predicate);
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
