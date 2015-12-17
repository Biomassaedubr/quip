#include "Document.hpp"

#include "Selection.hpp"
#include "SelectionSet.hpp"

#include <regex>

namespace {
  static std::regex gNewlinePattern("\\n");
}

namespace quip {
  Document::Document () {
  }
  
  Document::Document (const std::string & content)
  : m_rows(std::sregex_token_iterator(content.begin(), content.end(), gNewlinePattern, -1), std::sregex_token_iterator()) {
  }
  
  const std::string & Document::row (std::size_t index) const {
    return m_rows[index];
  }
  
  std::size_t Document::rows () const {
    return m_rows.size();
  }
  
  void Document::insert (SelectionSet & selections, const std::string & text) {
    int rowDelta = 0;
    int columnDelta = 0;
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
}