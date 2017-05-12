#pragma once

#include "DocumentIterator.hpp"

namespace quip {
  // A traversal provides a mechanism for moving an iterator in a particular direction
  // (such as in document order, or in reverse document order).
  //
  // Traversals expose the concept of advancing an iterator in the traversal's direction or
  // retreating it (in the opposite of the traversal's direction). These operations don't
  // always correspond with the increment and decrement functionality of the iterator. For
  // example, a document order traversal will increment an iterator when advancing, but a
  // reverse-document-order traversal will decrement it instead.
  //
  // Traversals are useful for writing direction-agnostic selector code which can be used
  // to implement a "next" or "prior" version of that selector simply by providing a different
  // traversal instance.
  struct Traversal {
    DocumentIterator advance(const DocumentIterator& iterator) const {
      return m_advance(iterator);
    }
    
    const DocumentIterator& advanceTo() const {
      return m_advanceTo;
    }
    
    DocumentIterator retreat(const DocumentIterator& iterator) const {
      return m_retreat(iterator);
    }
    
    const DocumentIterator& retreatTo() const {
      return m_retreatTo;
    }
    
    // Move an iterator in the traversal direction while a predicate passes for the character referred
    // to by the iterator.
    //
    // The resulting iterator is at the same position as the input if the predicate fails for the
    // character initially referred to. Otherwise, the resulting iterator will refer to the last
    // character encountered that passed the predicate.
    template<typename PredicateType>
    DocumentIterator advanceWhile(const DocumentIterator& iterator, PredicateType predicate) const;
    
    // Move an iterator in the traversal direction until a predicate passes for the character referred
    // to by the iterator.
    //
    // The resulting iterator will refer to the first character encountered that passed the predicate.
    template<typename PredicateType>
    DocumentIterator advanceUntil(const DocumentIterator& iterator, PredicateType predicate) const;
    
    // Move an iterator in the direction opposite the traversal direction while a predicate passes for
    // the character referred to by the iterator.
    //
    // The resulting iterator is at the same position as the input if the predicate fails for the
    // character initially referred to. Otherwise, the resulting iterator will refer to the first
    // character encountered that passed the predicate.
    template<typename PredicateType>
    DocumentIterator retreatWhile(const DocumentIterator& iterator, PredicateType predicate) const;
    
    // Move an iterator in the direction opposite the traversal direction until a predicate passes for
    // the character referred to by the iterator.
    //
    // The resulting iterator will refer to the first character encountered that passed the predicate.
    template<typename PredicateType>
    DocumentIterator retreatUntil(const DocumentIterator& iterator, PredicateType predicate) const;
    
    static Traversal documentOrder(const Document& document);
    static Traversal reverseDocumentOrder(const Document& document);
    
  private:
    typedef std::function<DocumentIterator (const DocumentIterator&)> MovementFunction;
    
    MovementFunction m_advance;
    MovementFunction m_retreat;
    DocumentIterator m_advanceTo;
    DocumentIterator m_retreatTo;
    
    Traversal(MovementFunction advanceFunction, const DocumentIterator& advanceTo, MovementFunction retreatFunction, const DocumentIterator& retreatTo)
    : m_advance(advanceFunction)
    , m_retreat(retreatFunction)
    , m_advanceTo(advanceTo)
    , m_retreatTo(retreatTo) {
    }
    
    static DocumentIterator nextInDocumentOrder(const DocumentIterator& iterator) {
      return std::next(iterator);
    }
    
    static DocumentIterator priorInDocumentOrder(const DocumentIterator& iterator) {
      return std::prev(iterator);
    }
  };
}

#include "Traversal.inl"
