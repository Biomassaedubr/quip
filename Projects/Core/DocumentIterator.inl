#include "Document.hpp"

namespace quip {
  template<typename PredicateType>
  DocumentIterator& DocumentIterator::advanceWhile(PredicateType predicate) {
    return moveWhile([](auto x) { return std::next(x); }, predicate);
  }
  
  template<typename PredicateType>
  DocumentIterator& DocumentIterator::advanceUntil(PredicateType predicate) {
    return moveUntil([](auto x) { return std::next(x); }, predicate);
  }
  
  template<typename PredicateType>
  DocumentIterator& DocumentIterator::reverseWhile(PredicateType predicate) {
    return moveWhile([](auto x) { return std::prev(x); }, predicate);
  }
  
  template<typename PredicateType>
  DocumentIterator& DocumentIterator::reverseUntil(PredicateType predicate) {
    return moveUntil([](auto x) { return std::prev(x); }, predicate);
  }
  
  template<typename MoveType, typename PredicateType>
  DocumentIterator& DocumentIterator::moveWhile(MoveType move, PredicateType predicate) {
    DocumentIterator cursor = *this;
    bool pass = predicate(*cursor);
    while (pass && cursor != m_document->begin() && cursor != m_document->end()) {
      DocumentIterator speculative = move(cursor);
      pass = predicate(*speculative);
      if(pass) {
        cursor = speculative;
      }
    }
    
    m_location = cursor.location();
    return *this;
  }
  
  template<typename MoveType, typename PredicateType>
  DocumentIterator& DocumentIterator::moveUntil(MoveType move, PredicateType predicate) {
    DocumentIterator cursor = *this;
    while(!predicate(*cursor) && cursor != m_document->begin() && cursor != m_document->end()) {
      cursor = move(cursor);
    }
    
    m_location = cursor.location();
    return *this;
  }
}
