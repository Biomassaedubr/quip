#pragma once

#include "DocumentIterator.hpp"

namespace quip {
  namespace Traversal {
    // Move an iterator in the traversal direction while a predicate passes for the character referred
    // to by the iterator.
    //
    // The resulting iterator is at the same position as the input if the predicate fails for the
    // character initially referred to. Otherwise, the resulting iterator will refer to the last
    // character encountered that passed the predicate.
    template<typename IteratorType, typename PredicateType>
    inline IteratorType advanceWhile(const IteratorType& iterator, PredicateType predicate) {
      IteratorType cursor = iterator;
      bool pass = predicate(*cursor);
      while (pass && !cursor.isEnd()) {
        IteratorType speculative = std::next(cursor);
        pass = predicate(*speculative);
        if(pass) {
          cursor = speculative;
        }
      }
      
      return cursor;
    }
    
    // Move an iterator in the traversal direction until a predicate passes for the character referred
    // to by the iterator.
    //
    // The resulting iterator will refer to the first character encountered that passed the predicate.
    template<typename IteratorType, typename PredicateType>
    inline IteratorType advanceUntil(const IteratorType& iterator, PredicateType predicate) {
      DocumentIterator cursor = iterator;
      while(!predicate(*cursor) && !cursor.isEnd()) {
        cursor = std::next(cursor);
      }
      
      return cursor;
    }
    
    // Move an iterator in the direction opposite the traversal direction while a predicate passes for
    // the character referred to by the iterator.
    //
    // The resulting iterator is at the same position as the input if the predicate fails for the
    // character initially referred to. Otherwise, the resulting iterator will refer to the first
    // character encountered that passed the predicate.
    template<typename IteratorType, typename PredicateType>
    inline IteratorType retreatWhile(const IteratorType& iterator, PredicateType predicate) {
      IteratorType cursor = iterator;
      bool pass = predicate(*cursor);
      while (pass && !cursor.isBegin()) {
        IteratorType speculative = std::prev(cursor);
        pass = predicate(*speculative);
        if(pass) {
          cursor = speculative;
        }
      }
      
      return cursor;
    }
    
    // Move an iterator in the direction opposite the traversal direction until a predicate passes for
    // the character referred to by the iterator.
    //
    // The resulting iterator will refer to the first character encountered that passed the predicate.
    template<typename IteratorType, typename PredicateType>
    inline IteratorType retreatUntil(const IteratorType& iterator, PredicateType predicate) {
      IteratorType cursor = iterator;
      while(!predicate(*cursor) && !cursor.isBegin()) {
        cursor = std::prev(cursor);
      }
      
      return cursor;
    }
  };
}
