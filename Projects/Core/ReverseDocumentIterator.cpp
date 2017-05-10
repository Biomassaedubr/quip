#include "ReverseDocumentIterator.hpp"

namespace quip {
  ReverseDocumentIterator::ReverseDocumentIterator(const DocumentIterator& iterator)
  : m_iterator(iterator) {
  }
  
  const Location& ReverseDocumentIterator::location() const {
    return m_iterator.location();
  }
  
  char ReverseDocumentIterator::operator*() const {
    DocumentIterator actual = m_iterator;
    --actual;
    return *actual;
  }
  
  ReverseDocumentIterator& ReverseDocumentIterator::operator++() {
    --m_iterator;
    return *this;
  }
  
  ReverseDocumentIterator& ReverseDocumentIterator::operator--() {
    ++m_iterator;
    return *this;
  }
  
  template<typename PredicateType>
  ReverseDocumentIterator& ReverseDocumentIterator::advanceWhile(PredicateType predicate) {
    m_iterator.reverseWhile(predicate);
    return *this;
  }
  
  template<typename PredicateType>
  ReverseDocumentIterator& ReverseDocumentIterator::reverseWhile(PredicateType predicate) {
    m_iterator.advanceWhile(predicate);
    return *this;
  }
  
  bool ReverseDocumentIterator::operator==(const ReverseDocumentIterator& other) {
    return m_iterator == other.m_iterator;
  }
  
  bool ReverseDocumentIterator::operator!=(const ReverseDocumentIterator& other) {
    return !(*this == other);
  }
}
