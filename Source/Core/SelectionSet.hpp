#pragma once

#include "Selection.hpp"
#include "SelectionSetIterator.hpp"

#include <vector>

namespace quip {
  struct SelectionSet {
    SelectionSet ();
    SelectionSet (const Selection & selection);
    SelectionSet (const std::vector<Selection> & selections);
    SelectionSet (const SelectionSet & other);
    SelectionSet (SelectionSet && other);
    
    SelectionSet & operator= (const SelectionSet & other);
    SelectionSet & operator= (SelectionSet && other);

    std::size_t count () const;
    
    const Selection & primary () const;
    
    Selection & operator[] (std::size_t index);
    const Selection & operator[] (std::size_t index) const;
    
    SelectionSetIterator begin ();
    ConstSelectionSetIterator begin () const;
    SelectionSetIterator end ();
    ConstSelectionSetIterator end () const;

    void rotateForward ();
    void rotateBackward ();
    
    void replace (const Selection & primary);
    void replace (const SelectionSet & selections);
    
  private:
    std::vector<Selection> m_selections;
    std::size_t m_primary;
  };
}