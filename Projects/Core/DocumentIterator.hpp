#pragma once

#include "Location.hpp"

#include <cstddef>
#include <iterator>

#include <functional>

namespace quip {
  struct Document;
  struct Traversal;
  
  // An iterator for a document, providing character-by-character traversal of the content
  // of a document.
  //
  // Document iterators only provide read-only access to the underlying document.
  struct DocumentIterator {
    typedef std::int64_t difference_type;
    typedef const char value_type;
    typedef const char* pointer;
    typedef const char& reference;
    typedef std::bidirectional_iterator_tag iterator_category;
    
    DocumentIterator(const Document& document, const Location& location);
    
    const Location& location() const;
    
    char operator*() const;
    
    DocumentIterator& operator++();
    DocumentIterator operator++(int);
    DocumentIterator& operator--();
    DocumentIterator operator--(int);
    
    DocumentIterator& advanceByRows(std::uint64_t rows);
    DocumentIterator& reverseByRows(std::uint64_t rows);
    
    // Advance the iterator while a predicate passes for the character referred to by the iterator.
    //
    // The iterator is not updated if the predicate fails for the character initially referred to.
    // Otherwise, the iterator will refer to the last character that passed the predicate.
    template<typename PredicateType>
    DocumentIterator& advanceWhile(PredicateType predicate);
    
    // Advance the iterator until a predicate passes for the character referred to by the iterator.
    //
    // The iterator is not updated if the predicate passes for the character initially referred to.
    // Otherwise, the iterator will refer to the first character that passed the predicate.
    template<typename PredicateType>
    DocumentIterator& advanceUntil(PredicateType predicate);
    
    // Reverse the iterator while a predicate passes for the character referred to by the iterator.
    //
    // The iterator is not updated if the predicate fails for the character initially referred to.
    // Otherwise, the iterator will refer to the last character that passed the predicate.
    template<typename PredicateType>
    DocumentIterator& reverseWhile(PredicateType predicate);
    
    // Reverse the iterator until a predicate passes for the character referred to by the iterator.
    //
    // The iterator is not updated if the predicate passes for the character initially referred to.
    // Otherwise, the iterator will refer to the first character that passed the predicate.
    template<typename PredicateType>
    DocumentIterator& reverseUntil(PredicateType predicate);
    
    bool operator==(const DocumentIterator& other);
    bool operator!=(const DocumentIterator& other);
    
  private:
    const Document* m_document;
    Location m_location;    
  };
  
  // A direction-agnostic mechanism for moving an iterator.
  struct Traversal {
    typedef std::function<DocumentIterator (const DocumentIterator&)> MovementFunction;
    
    Traversal(MovementFunction advanceFunction, const DocumentIterator& advanceTo, MovementFunction retreatFunction, const DocumentIterator& retreatTo)
    : m_advance(advanceFunction)
    , m_retreat(retreatFunction)
    , m_advanceGoal(advanceTo)
    , m_retreatGoal(retreatTo) {
    }
    
    DocumentIterator advance(const DocumentIterator& iterator) const {
      return m_advance(iterator);
    }
    
    const DocumentIterator& advanceTo() const {
      return m_advanceGoal;
    }
    
    DocumentIterator retreat(const DocumentIterator& iterator) const {
      return m_retreat(iterator);
    }
    
    const DocumentIterator& retreatTo() const {
      return m_retreatGoal;
    }
    
    template<typename PredicateType>
    DocumentIterator advanceWhile(const DocumentIterator& iterator, PredicateType predicate) const {
      DocumentIterator cursor = iterator;
      bool pass = predicate(*cursor);
      while (pass && cursor != m_advanceGoal) {
        DocumentIterator speculative = advance(cursor);
        pass = predicate(*speculative);
        if(pass) {
          cursor = speculative;
        }
      }
      
      return cursor;
    }
    
    template<typename PredicateType>
    DocumentIterator retreatWhile(const DocumentIterator& iterator, PredicateType predicate) const {
      DocumentIterator cursor = iterator;
      bool pass = predicate(*cursor);
      while (pass && cursor != m_retreatGoal) {
        DocumentIterator speculative = retreat(cursor);
        pass = predicate(*speculative);
        if(pass) {
          cursor = speculative;
        }
      }
      
      return cursor;
    }
    
    template<typename PredicateType>
    DocumentIterator advanceUntil(const DocumentIterator& iterator, PredicateType predicate) const {
      DocumentIterator cursor = iterator;
      while(!predicate(*cursor) && cursor != m_advanceGoal) {
        cursor = advance(cursor);
      }
      
      return cursor;
    }
    
    static Traversal documentOrder(const Document& document);
    static Traversal reverseDocumentOrder(const Document& document);
    
  private:
    MovementFunction m_advance;
    MovementFunction m_retreat;
    DocumentIterator m_advanceGoal;
    DocumentIterator m_retreatGoal;
    
    static DocumentIterator nextInDocumentOrder(const DocumentIterator& iterator) {
      return std::next(iterator);
    }
    
    static DocumentIterator priorInDocumentOrder(const DocumentIterator& iterator) {
      return std::prev(iterator);
    }
  };
}

#include "DocumentIterator.inl"
