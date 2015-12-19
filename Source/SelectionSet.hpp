#pragma once

#include "Selection.hpp"

#include <vector>

namespace quip {
  struct SelectionSet {
    SelectionSet (const Selection & selection);
    SelectionSet (const std::vector<Selection> & selections);

    Selection & primary ();
    
    Selection * begin ();
    const Selection * begin () const;
    Selection * end ();
    const Selection * end () const;
    
    void replace (const Selection & primary);
    void replace (const SelectionSet & selections);
    
  private:
    std::vector<Selection> m_selections;
    std::size_t m_primary;
  };
}