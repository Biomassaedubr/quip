#include "SelectionSet.hpp"

#include "Selection.hpp"

namespace quip {
  SelectionSet::SelectionSet (const Selection & selection)
  : m_selections({selection})
  , m_primary(0) {
  }
  
  Selection & SelectionSet::primary () {
    return m_selections[m_primary];
  }
  
  Selection * SelectionSet::begin () {
    return &m_selections.front();
  }
  
  Selection * SelectionSet::end () {
    return &m_selections.back() + 1;
  }
  
  void SelectionSet::replace (const Selection & primary) {
    m_selections.clear();
    m_selections.emplace_back(primary);
    m_primary = 0;
  }
}
