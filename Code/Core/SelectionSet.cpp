#include "SelectionSet.hpp"

#include "Selection.hpp"

namespace {
  static bool compareSelectionsByLowestLocation (const quip::Selection & left, const quip::Selection & right) {
    return left.origin() < right.origin();
  }
}

namespace quip {
  SelectionSet::SelectionSet()
  : m_primary(0) {
  }
  
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
        if (m_selections[basis].extent() >= m_selections[candidate].origin()) {
          // The two selections overlap and should be collapsed. Subsequent selections may also overlap the new combined
          // selection, so only the candidate index is advanced.
          m_selections[basis] = Selection(m_selections[basis].origin(), m_selections[candidate].extent());
          ++candidate;
        } else {
          // The selections don't overlap, so finalize the first selection and move both indices along.
          collapsed.emplace_back(m_selections[basis]);
          basis = candidate++;
        }
        
        if (candidate >= m_selections.size()) {
          collapsed.emplace_back(m_selections[basis]);
        }
      }

      m_selections = collapsed;
    }
  }
  
  SelectionSet::SelectionSet (const SelectionSet & other)
  : m_selections(other.m_selections)
  , m_primary(other.m_primary) {
  }
  
  SelectionSet::SelectionSet (SelectionSet && other)
  : m_selections(std::move(other.m_selections))
  , m_primary(other.m_primary) {
    other.m_primary = 0;
  }
  
  SelectionSet & SelectionSet::operator= (const SelectionSet & other) {
    m_selections = other.m_selections;
    m_primary = other.m_primary;
    return *this;
  }
  
  SelectionSet & SelectionSet::operator= (SelectionSet && other) {
    m_selections = std::move(other.m_selections);
    m_primary = other.m_primary;
    other.m_primary = 0;
    return *this;
  }
  
  std::size_t SelectionSet::count () const {
    return m_selections.size();
  }
  
  const Selection & SelectionSet::primary () const {
    return m_selections[m_primary];
  }
  
  Selection & SelectionSet::operator[] (std::size_t index) {
    return m_selections[index];
  }
  
  const Selection & SelectionSet::operator[] (std::size_t index) const {
    return m_selections[index];
  }
  
  SelectionSetIterator SelectionSet::begin () {
    return SelectionSetIterator(*this, 0);
  }
  
  ConstSelectionSetIterator SelectionSet::begin () const {
    return ConstSelectionSetIterator(*this, 0);
  }
  
  SelectionSetIterator SelectionSet::end () {
    return SelectionSetIterator(*this, m_selections.size());
  }
  
  ConstSelectionSetIterator SelectionSet::end () const {
    return ConstSelectionSetIterator(*this, m_selections.size());
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
