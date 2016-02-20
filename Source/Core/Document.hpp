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
    Document (const std::string & contents);
    
    std::string contents () const;
    std::string contents (const Selection & selection) const;
    
    DocumentIterator begin () const;
    DocumentIterator end () const;
    DocumentIterator at (const Location & location) const;
    
    const std::string & path () const;
    void setPath (const std::string & path);
    
    const Syntax * syntax () const;
    
    std::size_t rows () const;
    const std::string & row (std::size_t index) const;
    
    Location clip (const Location & location) const;
    
    SelectionSet insert (const SelectionSet & selections, const std::string & text);
    SelectionSet erase (const SelectionSet & selections);
    
    SelectionSet matches (const SearchExpression & expression) const;
    
  private:
    std::string m_path;
    std::unique_ptr<Syntax> m_syntax;
    
    std::vector<std::string> m_rows;
    
    std::vector<std::size_t> buildSpanTable (std::string * contents) const;
    Location linearPositionToLocation (const std::vector<std::size_t> & spanTable, std::size_t position) const;
  };
}