#pragma once

#include "Location.hpp"

#include <string>
#include <vector>

namespace quip {
  struct SelectionSet;
  
  struct Document {
    Document ();
    Document (const std::string & contents);
    
    const std::string & path () const;
    
    std::size_t rows () const;
    const std::string & row (std::size_t index) const;
    
    void insert (SelectionSet & selections, const std::string & text);
    
    SelectionSet matches (const std::string & pattern) const;
    
  private:
    std::vector<std::string> m_rows;
  };
}