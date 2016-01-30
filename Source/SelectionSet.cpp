#include "SelectionSet.hpp"

#include "Selection.hpp"

namespace {
  static bool compareSelectionsByLowestLocation (const quip::Selection & left, const quip::Selection & right) {
    return left.lowerBound() < right.lowerBound();
  }
}

namespace quip {
  SelectionSet::SelectionSet (const Selection & selection)
  : m_selections({selection})
  , m_primary(0) {
  }
  
  SelectionSet::SelectionSet (const std::vector<Selection> & selections)
  : m_selections(selections)
  , m_primary(0) {
    std::sort(m_selections.begin(), m_selections.end(), compareSelectionsByLowestLocation);
    
    if (m_selections.size() >= 2) {
      std::vector<Selection> collapsed;
      collapsed.reserve(selections.size());
      
      std::size_t basis = 0;
      std::size_t candidate = 1;
      while (basis < m_selections.size() && candidate < m_selections.size()) {
        if (m_selections[basis].upperBound() >= m_selections[candidate].lowerBound()) {
          m_selections[basis] = Selection(m_selections[basis].lowerBound(), m_selections[candidate].upperBound());
          ++candidate;
          if (candidate >= m_selections.size()) {
            // Ensure a collapse of the last candidate selection gets recorded.
            collapsed.emplace_back(m_selections[basis]);
          }
        } else {
          collapsed.emplace_back(m_selections[basis]);
          basis = candidate++;
        }
      }
      
      m_selections = collapsed;
    }
  }
  
  std::size_t SelectionSet::count () const {
    return m_selections.size();
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
  
  void SelectionSet::rotateForward () {
    m_primary = (m_primary + 1) % m_selections.size();
  }
  
  void SelectionSet::rotateBackward () {
    if (m_primary == 0) {
      m_primary = m_selections.size() - 1;
    } else {
      m_primary -= 1;
    }
  }
  
  void SelectionSet::replace (const Selection & primary) {
    m_selections.clear();
    m_selections.emplace_back(primary);
    m_primary = 0;
  }
  
  void SelectionSet::replace (const SelectionSet & selections) {
    m_selections.clear();
    
    // The source selection set will already be sorted and collapsed.
    m_selections.insert(m_selections.begin(), selections.begin(), selections.end());
    m_primary = selections.m_primary;
  }
}
