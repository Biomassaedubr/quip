namespace quip {
  namespace Detail {
    template<typename SetType, typename ValueType>
    SelectionSetIteratorTemplate<SetType, ValueType>::SelectionSetIteratorTemplate (SetType & selections, std::size_t index)
    : m_selections(&selections)
    , m_index(index) {
    }
    
    template<typename SetType, typename ValueType>
    ValueType & SelectionSetIteratorTemplate<SetType, ValueType>::operator* () const {
      return (*m_selections)[m_index];
    }
    
    template<typename SetType, typename ValueType>
    ValueType * SelectionSetIteratorTemplate<SetType, ValueType>::operator-> () const {
      return &(*m_selections)[m_index];
    }
    
    template<typename SetType, typename ValueType>
    SelectionSetIteratorTemplate<SetType, ValueType> & SelectionSetIteratorTemplate<SetType, ValueType>::operator++ () {
      ++m_index;
      return *this;
    }
    
    template<typename SetType, typename ValueType>
    SelectionSetIteratorTemplate<SetType, ValueType> & SelectionSetIteratorTemplate<SetType, ValueType>::operator-- () {
      --m_index;
      return *this;
    }
    
    template<typename SetType, typename ValueType>
    bool SelectionSetIteratorTemplate<SetType, ValueType>::operator== (const SelectionSetIteratorTemplate<SetType, ValueType> & other) const {
      return m_selections == other.m_selections && m_index == other.m_index;
    }
    
    template<typename SetType, typename ValueType>
    bool SelectionSetIteratorTemplate<SetType, ValueType>::operator!= (const SelectionSetIteratorTemplate<SetType, ValueType> & other) const {
      return !(*this == other);
    }
  }
}