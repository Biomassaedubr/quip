#include "Traversal.hpp"

#include "Document.hpp"

namespace quip {
  Traversal::Traversal(MovementFunction advanceFunction, const DocumentIterator& advanceTo, MovementFunction retreatFunction, const DocumentIterator& retreatTo)
  : m_advance(advanceFunction)
  , m_retreat(retreatFunction)
  , m_advanceTo(advanceTo)
  , m_retreatTo(retreatTo) {
  }
  
  DocumentIterator Traversal::advance(const DocumentIterator& iterator) const {
    return m_advance(iterator);
  }
 
  DocumentIterator Traversal::retreat(const DocumentIterator& iterator) const {
    return m_retreat(iterator);
  }
  
  const DocumentIterator& Traversal::advanceTo() const {
    return m_advanceTo;
  }
  
  const DocumentIterator& Traversal::retreatTo() const {
    return m_retreatTo;
  }
  
  Traversal Traversal::documentOrder(const Document& document) {
    return Traversal(&nextInDocumentOrder, document.end(), &priorInDocumentOrder, document.begin());
  }
  
  Traversal Traversal::reverseDocumentOrder(const Document& document) {
    return Traversal(priorInDocumentOrder, document.begin(), nextInDocumentOrder, document.end());
  }
  
  DocumentIterator Traversal::nextInDocumentOrder(const DocumentIterator& iterator) {
    return std::next(iterator);
  }
  
  DocumentIterator Traversal::priorInDocumentOrder(const DocumentIterator& iterator) {
    return std::prev(iterator);
  }
}
