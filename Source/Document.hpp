#pragma once

#include "Location.hpp"

#include <string>
#include <vector>

namespace quip {
  struct SelectionSet;
  
  struct Document {
    Document ();
    Document (const std::string & contents);
    
    std::string contents () const;
    
    const std::string & path () const;
    void setPath (const std::string & path);
    
    std::size_t rows () const;
    const std::string & row (std::size_t index) const;
    
    void insert (SelectionSet & selections, const std::string & text);
    
    SelectionSet matches (const std::string & pattern) const;
    
  private:
    std::string m_path;
    std::vector<std::string> m_rows;
  };
}