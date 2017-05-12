namespace quip {
  template<typename PredicateType>
  DocumentIterator Traversal::advanceWhile(const DocumentIterator& iterator, PredicateType predicate) const {
    DocumentIterator cursor = iterator;
    bool pass = predicate(*cursor);
    while (pass && cursor != m_advanceTo) {
      DocumentIterator speculative = advance(cursor);
      pass = predicate(*speculative);
      if(pass) {
        cursor = speculative;
      }
    }
    
    return cursor;
  }
  
  template<typename PredicateType>
  DocumentIterator Traversal::advanceUntil(const DocumentIterator& iterator, PredicateType predicate) const {
    DocumentIterator cursor = iterator;
    while(!predicate(*cursor) && cursor != m_advanceTo) {
      cursor = advance(cursor);
    }
    
    return cursor;
  }

  template<typename PredicateType>
  DocumentIterator Traversal::retreatWhile(const DocumentIterator& iterator, PredicateType predicate) const {
    DocumentIterator cursor = iterator;
    bool pass = predicate(*cursor);
    while (pass && cursor != m_retreatTo) {
      DocumentIterator speculative = retreat(cursor);
      pass = predicate(*speculative);
      if(pass) {
        cursor = speculative;
      }
    }
    
    return cursor;
  }

  template<typename PredicateType>
  DocumentIterator Traversal::retreatUntil(const DocumentIterator& iterator, PredicateType predicate) const {
    DocumentIterator cursor = iterator;
    while(!predicate(*cursor) && cursor != m_retreatTo) {
      cursor = retreat(cursor);
    }
    
    return cursor;
  }
}
