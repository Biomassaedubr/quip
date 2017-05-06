#pragma once

#include "Location.hpp"
#include "Signal.hpp"

#include <string>
#include <vector>

namespace quip {
  struct SearchExpression;
  struct Selection;
  struct SelectionSet;

  template<typename ElementType, bool IsConst>
  struct DocumentIterator;
  
  struct Document {
    typedef DocumentIterator<char, false> Iterator;
    typedef DocumentIterator<char, true> ConstIterator;
    typedef std::reverse_iterator<Iterator> ReverseIterator;
    typedef std::reverse_iterator<ConstIterator> ConstReverseIterator;
    
    Document();
    explicit Document(const std::string& contents);
    
    bool isEmpty() const noexcept;
    bool isMissingTrailingNewline() const noexcept;
        
    std::string contents() const;
    std::string contents(const Selection& selection) const;
    std::vector<std::string> contents(const SelectionSet& selection) const;
    
    Iterator begin();
    ConstIterator begin() const;
    ReverseIterator rbegin();
    ConstReverseIterator rbegin() const;
    
    Iterator end();
    ConstIterator end() const;
    ReverseIterator rend();
    ConstReverseIterator rend() const;
    
    Iterator from(const Location& location);
    Iterator from(std::uint64_t column, std::uint64_t row);
    ConstIterator from(const Location& location) const;
    ConstIterator from(std::uint64_t column, std::uint64_t row) const;

    ReverseIterator rfrom(const Location& location);
    ReverseIterator rfrom(std::uint64_t column, std::uint64_t row);
    ConstReverseIterator rfrom(const Location& location) const;
    ConstReverseIterator rfrom(std::uint64_t column, std::uint64_t row) const;
    
    std::int64_t distance(const Location& from, const Location& to) const;
    
    const std::string& path() const;
    void setPath(const std::string& path);

    std::size_t rows() const;
    const std::string& row(std::size_t index) const;
    
    std::string indentOfRow(std::size_t index) const;

    SelectionSet insert(const Selection& selection, const std::string& text);
    SelectionSet insert(const SelectionSet& selections, const std::string& text);
    SelectionSet insert(const SelectionSet& selections, const std::vector<std::string>& text);

    SelectionSet append(const Selection& selection, const std::string& text);
    SelectionSet append(const SelectionSet& selections, const std::string& text);
    SelectionSet append(const SelectionSet& selections, const std::vector<std::string>& text);

    SelectionSet erase(const Selection& selection);
    SelectionSet erase(const SelectionSet& selections);
    
    SelectionSet matches(const SearchExpression& expression) const;
        
    Signal<void()>& onDocumentModified();
    
  private:
    std::string m_path;    
    std::vector<std::string> m_rows;
    
    Signal<void()> m_documentModifiedSignal;
    
    std::vector<std::string> decompose(const std::string& text) const;
    
    std::vector<std::size_t> buildSpanTable(std::string* contents) const;
    Location linearPositionToLocation(const std::vector<std::size_t>& spanTable, std::size_t position) const;
  };
}
