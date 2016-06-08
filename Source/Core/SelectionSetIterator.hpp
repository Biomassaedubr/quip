#pragma once

#include <cstddef>
#include <iterator>

namespace quip {
  namespace Detail {
    template<typename SetType, typename ValueType>
    struct SelectionSetIteratorTemplate : std::iterator<std::bidirectional_iterator_tag, ValueType> {
      SelectionSetIteratorTemplate (SetType & selections, std::size_t index);
      
      ValueType & operator* () const;
      ValueType * operator-> () const;
      
      SelectionSetIteratorTemplate & operator++ ();
      SelectionSetIteratorTemplate & operator-- ();
      
      bool operator== (const SelectionSetIteratorTemplate & other) const;
      bool operator!= (const SelectionSetIteratorTemplate & other) const;
    private:
      SetType * m_selections;
      std::size_t m_index;
    };
  }
  
  struct Selection;
  struct SelectionSet;
  
  typedef Detail::SelectionSetIteratorTemplate<SelectionSet, Selection> SelectionSetIterator;
  typedef Detail::SelectionSetIteratorTemplate<const SelectionSet, const Selection> ConstSelectionSetIterator;
}

#include "SelectionSetIterator.inl"