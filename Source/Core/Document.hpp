#pragma once

#include "Location.hpp"
#include "Syntax.hpp"

#include <string>
#include <vector>

namespace quip {
  struct DocumentIterator;
  struct SearchExpression;
  struct Selection;
  struct SelectionSet;

  struct Document {
    Document ();
    explicit Document (const std::string & contents);
    
    bool isEmpty () const noexcept;
    bool isMissingTrailingNewline () const noexcept;
        
    std::string contents () const;
    std::string contents (const Selection & selection) const;
    std::vector<std::string> contents (const SelectionSet & selection) const;
    
    DocumentIterator begin () const;
    DocumentIterator end () const;
    DocumentIterator at (const Location & location) const;
        
    std::int64_t distance (const Location & from, const Location & to) const;
    
    const std::string & path () const;
    void setPath (const std::string & path);

    std::size_t rows () const;
    const std::string & row (std::size_t index) const;

    SelectionSet insert (const Selection & selection, const std::string & text);
    SelectionSet insert (const SelectionSet & selections, const std::string & text);
    SelectionSet insert (const SelectionSet & selections, const std::vector<std::string> & text);

    SelectionSet erase (const Selection & selection);
    SelectionSet erase (const SelectionSet & selections);
    
    SelectionSet matches (const SearchExpression & expression) const;
    
    std::vector<AttributeRange> highlight (std::uint64_t row) const;
    
  private:
    std::string m_path;
    SyntaxParseFunction m_syntaxParseFunction;
    
    std::vector<std::string> m_rows;
    
    std::vector<std::string> decompose (const std::string & text) const;
    
    std::vector<std::size_t> buildSpanTable (std::string * contents) const;
    Location linearPositionToLocation (const std::vector<std::size_t> & spanTable, std::size_t position) const;
  };
}