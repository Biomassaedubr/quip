#include "Document.hpp"

namespace quip {
  template<typename PredicateType>
  DocumentIterator& DocumentIterator::advanceWhile(PredicateType predicate) {
    DocumentIterator result = Traversal::documentOrder(*m_document).advanceWhile(*this, predicate);
    *this = result;
    return *this;
  }
  
  template<typename PredicateType>
  DocumentIterator& DocumentIterator::advanceUntil(PredicateType predicate) {
    DocumentIterator result = Traversal::documentOrder(*m_document).advanceUntil(*this, predicate);
    *this = result;
    return *this;  }
  
  template<typename PredicateType>
  DocumentIterator& DocumentIterator::reverseWhile(PredicateType predicate) {
    DocumentIterator result = Traversal::reverseDocumentOrder(*m_document).advanceWhile(*this, predicate);
    *this = result;
    return *this;
  }
  
  template<typename PredicateType>
  DocumentIterator& DocumentIterator::reverseUntil(PredicateType predicate) {
    DocumentIterator result = Traversal::reverseDocumentOrder(*m_document).advanceUntil(*this, predicate);
    *this = result;
    return *this;
  }
}
