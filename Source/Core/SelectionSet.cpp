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
          // The two selections overlap and should be collapsed. Subsequent selections may also overlap the new combined
          // selection, so only the candidate index is advanced.
          m_selections[basis] = Selection(m_selections[basis].lowerBound(), m_selections[candidate].upperBound());
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
  
  std::size_t SelectionSet::count () const {
    return m_selections.size();
  }
  
  Selection & SelectionSet::primary () {
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
  
  ReverseSelectionSetIterator SelectionSet::rbegin () {
    return ReverseSelectionSetIterator(*this, m_selections.size() - 1);
  }
  
  ReverseConstSelectionSetIterator SelectionSet::rbegin () const {
    return ReverseConstSelectionSetIterator(*this, m_selections.size() - 1);
  }
  
  ReverseSelectionSetIterator SelectionSet::rend () {
    return ReverseSelectionSetIterator(*this, -1);
  }
  
  ReverseConstSelectionSetIterator SelectionSet::rend () const {
    return ReverseConstSelectionSetIterator(*this, -1);
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