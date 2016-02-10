#pragma once

#include <cstddef>
#include <iterator>

namespace quip {
  struct Selection;
  struct SelectionSet;
  
  struct SelectionSetIterator {
    SelectionSetIterator ();
    SelectionSetIterator (SelectionSet & selections, std::size_t index);
    
    Selection & operator* ();
    Selection & operator* () const;
    
    Selection * operator-> ();
    Selection * operator-> () const;
    
    SelectionSetIterator & operator++ ();
    SelectionSetIterator & operator-- ();
    
    friend bool operator== (const SelectionSetIterator & left, const SelectionSetIterator & right);
    friend bool operator!= (const SelectionSetIterator & left, const SelectionSetIterator & right);
    
  private:
    SelectionSet * m_selections;
    std::size_t m_index;
  };
  
  struct ConstSelectionSetIterator {
    ConstSelectionSetIterator ();
    ConstSelectionSetIterator (const SelectionSet & selections, std::size_t index);
    
    const Selection & operator* ();
    const Selection & operator* () const;
    
    const Selection * operator-> ();
    const Selection * operator-> () const;
    
    ConstSelectionSetIterator & operator++ ();
    ConstSelectionSetIterator & operator-- ();
    
    friend bool operator== (const ConstSelectionSetIterator & left, const ConstSelectionSetIterator & right);
    friend bool operator!= (const ConstSelectionSetIterator & left, const ConstSelectionSetIterator & right);
    
  private:
    const SelectionSet * m_selections;
    std::size_t m_index;
  };
  
  struct ReverseSelectionSetIterator {
    ReverseSelectionSetIterator ();
    ReverseSelectionSetIterator (SelectionSet & selections, std::size_t index);
    
    Selection & operator* ();
    Selection & operator* () const;
    
    Selection * operator-> ();
    Selection * operator-> () const;
    
    ReverseSelectionSetIterator & operator++ ();
    ReverseSelectionSetIterator & operator-- ();
    
    friend bool operator== (const ReverseSelectionSetIterator & left, const ReverseSelectionSetIterator & right);
    friend bool operator!= (const ReverseSelectionSetIterator & left, const ReverseSelectionSetIterator & right);
    
  private:
    SelectionSet * m_selections;
    std::size_t m_index;
  };
  
  struct ReverseConstSelectionSetIterator {
    ReverseConstSelectionSetIterator ();
    ReverseConstSelectionSetIterator (const SelectionSet & selections, std::size_t index);
    
    const Selection & operator* ();
    const Selection & operator* () const;
    
    const Selection * operator-> ();
    const Selection * operator-> () const;
    
    ReverseConstSelectionSetIterator & operator++ ();
    ReverseConstSelectionSetIterator & operator-- ();
    
    friend bool operator== (const ReverseConstSelectionSetIterator & left, const ReverseConstSelectionSetIterator & right);
    friend bool operator!= (const ReverseConstSelectionSetIterator & left, const ReverseConstSelectionSetIterator & right);
    
  private:
    const SelectionSet * m_selections;
    std::size_t m_index;
  };
}

namespace std {
  template<>
  struct iterator_traits<quip::SelectionSetIterator> {
    typedef quip::Selection value_type;
    typedef std::ptrdiff_t difference_type;
    typedef quip::Selection * pointer;
    typedef quip::Selection & reference;
    typedef std::bidirectional_iterator_tag iterator_category;
  };
  
  template<>
  struct iterator_traits<quip::ConstSelectionSetIterator> {
    typedef const quip::Selection value_type;
    typedef std::ptrdiff_t difference_type;
    typedef const quip::Selection * pointer;
    typedef const quip::Selection & reference;
    typedef std::bidirectional_iterator_tag iterator_category;
  };
  
  template<>
  struct iterator_traits<quip::ReverseSelectionSetIterator> {
    typedef quip::Selection value_type;
    typedef std::ptrdiff_t difference_type;
    typedef quip::Selection * pointer;
    typedef quip::Selection & reference;
    typedef std::bidirectional_iterator_tag iterator_category;
  };
  
  template<>
  struct iterator_traits<quip::ReverseConstSelectionSetIterator> {
    typedef const quip::Selection value_type;
    typedef std::ptrdiff_t difference_type;
    typedef const quip::Selection * pointer;
    typedef const quip::Selection & reference;
    typedef std::bidirectional_iterator_tag iterator_category;
  };
}