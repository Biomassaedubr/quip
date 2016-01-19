#include "Document.hpp"

#include "DocumentIterator.hpp"
#include "SearchExpression.hpp"
#include "Selection.hpp"
#include "SelectionSet.hpp"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>

namespace quip {
  Document::Document () {
  }
  
  Document::Document (const std::string & content) {
    for (std::size_t index = 0; index < content.size(); ++index) {
      std::size_t start = index;
      while (content[index] != '\n' && index < content.size()) {
        ++index;
      }
      
      m_rows.emplace_back(content.substr(start, index - start + 1));
    }
  }
  
  std::string Document::contents () const {
    std::ostringstream stream;
    for (const std::string & text : m_rows) {
      stream << text;
    }
    
    return stream.str();
  }
  
  DocumentIterator Document::begin () {
    return DocumentIterator(*this, Location(0, 0));
  }
  
  DocumentIterator Document::end () {
    return DocumentIterator(*this, Location(0, m_rows.size()));
  }
  
  const std::string & Document::path () const {
    return m_path;
  }
  
  void Document::setPath (const std::string & path) {
    m_path = path;
  }
  
  const std::string & Document::row (std::size_t index) const {
    return m_rows[index];
  }
  
  std::size_t Document::rows () const {
    return m_rows.size();
  }
  
  void Document::insert (SelectionSet & selections, const std::string & text) {
    std::int64_t rowDelta = 0;
    std::int64_t columnDelta = 0;
    std::size_t priorRow = rows();
    
    for (Selection & selection : selections) {
      // Reset column delta when changing rows.
      if (selection.lowerBound().row() != priorRow) {
        priorRow = selection.lowerBound().row();
        columnDelta = 0;
      }
      
      Location lower = selection.lowerBound().adjustBy(columnDelta, rowDelta);
      std::string prefix = m_rows[lower.row()].substr(0, lower.column());
      std::string suffix = m_rows[lower.row()].substr(lower.column());
      m_rows[lower.row()] = prefix + text + suffix;
      
      columnDelta += text.size();
      
      selection.setOrigin(selection.origin().adjustBy(columnDelta, 0));
      selection.setExtent(selection.origin());
    }
  }
  
  void Document::erase (SelectionSet & selections) {
    std::int64_t rowDelta = 0;
    std::int64_t columnDelta = 0;
    std::size_t priorRow = rows();
    
    for (Selection & selection : selections) {
      // Reset column delta when changing rows.
      if (selection.lowerBound().row() != priorRow) {
        priorRow = selection.lowerBound().row();
        columnDelta = 0;
      }
      
      Location lower = selection.lowerBound().adjustBy(columnDelta, rowDelta);
      Location upper = selection.upperBound().adjustBy(columnDelta, rowDelta);
      std::string prefix = m_rows[lower.row()].substr(0, lower.column());
      std::string suffix = m_rows[upper.row()].substr(upper.column() + 1);
      m_rows[lower.row()] = prefix + suffix;
      
      std::uint64_t removed = selection.height() - 1;
      selection.setOrigin(selection.origin().adjustBy(columnDelta, 0));
      selection.setExtent(selection.origin());
      
      if (selection.height() == 1) {
        columnDelta -= (upper.column() - lower.column() + 1);
      } else {
        columnDelta = upper.column();
        while (removed > 0) {
          m_rows.erase(m_rows.begin() + (lower.row() + 1));
          --rowDelta;
          --removed;
        }
      }
    }
  }
  
  void Document::eraseBefore (SelectionSet & selections) {
    std::int64_t rowDelta = 0;
    std::int64_t columnDelta = 0;
    std::size_t priorRow = rows();
    
    for (Selection & selection : selections) {
      // Reset column delta when changing rows.
      if (selection.lowerBound().row() != priorRow) {
        priorRow = selection.lowerBound().row();
        columnDelta = 0;
      }
      
      Location lower = selection.lowerBound().adjustBy(columnDelta, rowDelta);
      Location upper = selection.upperBound().adjustBy(columnDelta, rowDelta);
      std::string prefix = m_rows[lower.row()].substr(0, lower.column() - 1);
      std::string suffix = m_rows[upper.row()].substr(upper.column());
      m_rows[lower.row()] = prefix + suffix;
      
      std::uint64_t removed = selection.height() - 1;
      selection.setOrigin(selection.origin().adjustBy(-1, 0));
      selection.setExtent(selection.origin());
      
      if (selection.height() == 1) {
        columnDelta -= (upper.column() - lower.column() + 1);
      } else {
        columnDelta = upper.column();
        while (removed > 0) {
          m_rows.erase(m_rows.begin() + (lower.row() + 1));
          --rowDelta;
          --removed;
        }
      }
    }
  }
  
  SelectionSet Document::matches (const SearchExpression & expression) const {
    std::vector<Selection> results;
    if (expression.valid()) {
      for (std::size_t row = 0; row < m_rows.size(); ++row) {
        const std::string & text = m_rows[row];
        std::sregex_iterator cursor(text.begin(), text.end(), expression.pattern(), std::regex_constants::match_not_null);
        std::sregex_iterator end;
        
        while (cursor != end) {
          std::smatch match = *cursor;
          if (match.length() == 0) {
            // https://llvm.org/bugs/show_bug.cgi?id=21597 notes that libc++ doesn't currently
            // respect the "ignore empty matches" flag passed above. This can cause partially-entered
            // expressions containing \b assertions to generate an infinite loop, since incrementing
            // the iterator will never actually advance it. As a workaround, matches for a row are
            // aborted if any are empty, since that should only occur in the context of the libc++ bug.
            break;
          }
          
          for (std::size_t matchIndex = 0; matchIndex < match.size(); ++matchIndex) {
            std::size_t origin = match.position(matchIndex);
            std::size_t extent = origin + match[matchIndex].length() - 1;
            results.emplace_back(Location(origin, row), Location(extent, row));
          }
          
          ++cursor;
        }
      }
    }
    
    return SelectionSet(results);
  }
}