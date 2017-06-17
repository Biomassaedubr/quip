#include "ReverseDocumentIterator.hpp"

#include "Document.hpp"

namespace quip {
  ReverseDocumentIterator::ReverseDocumentIterator(const DocumentIterator& basis)
  : m_basis(basis) {
  }
  
  const Document& ReverseDocumentIterator::document() const {
    return m_basis.document();
  }
  
  Location ReverseDocumentIterator::location() const {
    if (m_basis.location() == Location(0,0)) {
      return Location::invalid();
    }
    
    return std::prev(m_basis).location();
  }
  
  bool ReverseDocumentIterator::isBegin() const {
    return *this == document().rbegin();
  }
  
  bool ReverseDocumentIterator::isEnd() const {
    return *this == document().rend();
  }
  
  char ReverseDocumentIterator::operator*() const {
    return *std::prev(m_basis);
  }
  
  ReverseDocumentIterator& ReverseDocumentIterator::operator++() {
    --m_basis;
    return *this;
  }
  
  ReverseDocumentIterator ReverseDocumentIterator::operator++(int) {
    return ReverseDocumentIterator(std::prev(m_basis));
  }
  
  ReverseDocumentIterator& ReverseDocumentIterator::operator--() {
    ++m_basis;
    return *this;
  }
  
  ReverseDocumentIterator ReverseDocumentIterator::operator--(int) {
    return ReverseDocumentIterator(std::next(m_basis));
  }
  
  bool ReverseDocumentIterator::operator==(const ReverseDocumentIterator& other) const {
    return m_basis == other.m_basis;
  }
  
  bool ReverseDocumentIterator::operator!=(const ReverseDocumentIterator& other) const {
    return !(*this == other);
  }
}
