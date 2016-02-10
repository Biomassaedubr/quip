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
  
  ReverseSelectionSetIterator::ReverseSelectionSetIterator ()
  : m_selections(nullptr)
  , m_index(0) {
  }
  
  ReverseSelectionSetIterator::ReverseSelectionSetIterator (SelectionSet & selections, std::size_t index)
  : m_selections(&selections)
  , m_index(index) {
  }
  
  Selection & ReverseSelectionSetIterator::operator* () {
    return (*m_selections)[m_index];
  }
  
  Selection & ReverseSelectionSetIterator::operator* () const {
    return (*m_selections)[m_index];
  }
  
  Selection * ReverseSelectionSetIterator::operator-> () {
    return &(*m_selections)[m_index];
  }
  
  Selection * ReverseSelectionSetIterator::operator-> () const {
    return &(*m_selections)[m_index];
  }
  
  ReverseSelectionSetIterator & ReverseSelectionSetIterator::operator++ () {
    --m_index;
    return *this;
  }
  
  ReverseSelectionSetIterator & ReverseSelectionSetIterator::operator-- () {
    ++m_index;
    return *this;
  }
  
  bool operator== (const ReverseSelectionSetIterator & left, const ReverseSelectionSetIterator & right) {
    return left.m_selections == right.m_selections && left.m_index == right.m_index;
  }
  
  bool operator!= (const ReverseSelectionSetIterator & left, const ReverseSelectionSetIterator & right) {
    return !(left == right);
  }
  
  ReverseConstSelectionSetIterator::ReverseConstSelectionSetIterator ()
  : m_selections(nullptr)
  , m_index(0) {
  }
  
  ReverseConstSelectionSetIterator::ReverseConstSelectionSetIterator (const SelectionSet & selections, std::size_t index)
  : m_selections(&selections)
  , m_index(index) {
  }
  
  const Selection & ReverseConstSelectionSetIterator::operator* () {
    return (*m_selections)[m_index];
  }
  
  const Selection & ReverseConstSelectionSetIterator::operator* () const {
    return (*m_selections)[m_index];
  }
  
  const Selection * ReverseConstSelectionSetIterator::operator-> () {
    return &(*m_selections)[m_index];
  }
  
  const Selection * ReverseConstSelectionSetIterator::operator-> () const {
    return &(*m_selections)[m_index];
  }
  
  ReverseConstSelectionSetIterator & ReverseConstSelectionSetIterator::operator++ () {
    --m_index;
    return *this;
  }
  
  ReverseConstSelectionSetIterator & ReverseConstSelectionSetIterator::operator-- () {
    ++m_index;
    return *this;
  }
  
  bool operator== (const ReverseConstSelectionSetIterator & left, const ReverseConstSelectionSetIterator & right) {
    return left.m_selections == right.m_selections && left.m_index == right.m_index;
  }
  
  bool operator!= (const ReverseConstSelectionSetIterator & left, const ReverseConstSelectionSetIterator & right) {
    return !(left == right);
  }
}