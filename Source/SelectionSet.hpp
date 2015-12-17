#pragma once

#include <vector>

namespace quip {
  struct Selection;
  
  struct SelectionSet {
    SelectionSet (const Selection & selection);

    Selection & primary ();
    
    Selection * begin ();
    Selection * end ();
    
    void replace (const Selection & primary);
    
  private:
    std::vector<Selection> m_selections;
    std::size_t m_primary;
  };
}