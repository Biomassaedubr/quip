#include "Document.hpp"

namespace quip {
  template<typename PredicateType>
  DocumentIterator& DocumentIterator::advanceWhile(PredicateType predicate) {
    DocumentIterator& self = *this;
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
  
  template<typename PredicateType>
  DocumentIterator& DocumentIterator::advanceUntil(PredicateType predicate) {
    DocumentIterator& self = *this;
    while(self != m_document->end() && !predicate(*self)) {
      ++self;
    }
    
    return self;
  }
  
  template<typename PredicateType>
  DocumentIterator& DocumentIterator::reverseWhile(PredicateType predicate) {
    DocumentIterator& self = *this;
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
}
