#include "SelectionSetIterator.hpp"

#include "SelectionSet.hpp"

namespace quip {
  SelectionSetIterator::SelectionSetIterator ()
  : m_selections(nullptr)
  , m_index(0) {
  }
  
  SelectionSetIterator::SelectionSetIterator (SelectionSet & selections, std::size_t index)
  : m_selections(&selections)
  , m_index(index) {
  }
  
  Selection & SelectionSetIterator::operator* () {
    return (*m_selections)[m_index];
  }
  
  Selection & SelectionSetIterator::operator* () const {
    return (*m_selections)[m_index];
  }
  
  Selection * SelectionSetIterator::operator-> () {
    return &(*m_selections)[m_index];
  }
  
  Selection * SelectionSetIterator::operator-> () const {
    return &(*m_selections)[m_index];
  }
  
  SelectionSetIterator & SelectionSetIterator::operator++ () {
    ++m_index;
    return *this;
  }
  
  SelectionSetIterator & SelectionSetIterator::operator-- () {
    --m_index;
    return *this;
  }
  
  bool operator== (const SelectionSetIterator & left, const SelectionSetIterator & right) {
    return left.m_selections == right.m_selections && left.m_index == right.m_index;
  }
  
  bool operator!= (const SelectionSetIterator & left, const SelectionSetIterator & right) {
    return !(left == right);
  }
  
  ConstSelectionSetIterator::ConstSelectionSetIterator ()
  : m_selections(nullptr)
  , m_index(0) {
  }
  
  ConstSelectionSetIterator::ConstSelectionSetIterator (const SelectionSet & selections, std::size_t index)
  : m_selections(&selections)
  , m_index(index) {
  }
  
  const Selection & ConstSelectionSetIterator::operator* () {
    return (*m_selections)[m_index];
  }
  
  const Selection & ConstSelectionSetIterator::operator* () const {
    return (*m_selections)[m_index];
  }
  
  const Selection * ConstSelectionSetIterator::operator-> () {
    return &(*m_selections)[m_index];
  }
  
  const Selection * ConstSelectionSetIterator::operator-> () const {
    return &(*m_selections)[m_index];
  }
  
  ConstSelectionSetIterator & ConstSelectionSetIterator::operator++ () {
    ++m_index;
    return *this;
  }
  
  ConstSelectionSetIterator & ConstSelectionSetIterator::operator-- () {
    --m_index;
    return *this;
  }
  
  bool operator== (const ConstSelectionSetIterator & left, const ConstSelectionSetIterator & right) {
    return left.m_selections == right.m_selections && left.m_index == right.m_index;
  }
  
  bool operator!= (const ConstSelectionSetIterator & left, const ConstSelectionSetIterator & right) {
    return !(left == right);
  }
}