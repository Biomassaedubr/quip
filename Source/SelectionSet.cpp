#include "SelectionSet.hpp"

#include "Selection.hpp"

namespace quip {
  SelectionSet::SelectionSet (const Selection & selection)
  : m_selections({selection})
  , m_primary(0) {
  }
  
  SelectionSet::SelectionSet (const std::vector<Selection> & selections)
  : m_selections(selections)
  , m_primary(0) {
  }
  
  Selection & SelectionSet::primary () {
    return m_selections[m_primary];
  }
  
  Selection * SelectionSet::begin () {
    return &m_selections.front();
  }
  
  const Selection * SelectionSet::begin () const {
    return &m_selections.front();
  }
  
  Selection * SelectionSet::end () {
    return &m_selections.back() + 1;
  }
  
  const Selection * SelectionSet::end () const {
    return &m_selections.back() + 1;
  }
  
  void SelectionSet::replace (const Selection & primary) {
    m_selections.clear();
    m_selections.emplace_back(primary);
    m_primary = 0;
  }
  
  void SelectionSet::replace (const SelectionSet & selections) {
    m_selections.clear();
    m_selections.insert(m_selections.begin(), selections.begin(), selections.end());
    m_primary = selections.m_primary;
  }
}
