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
  namespace {
    std::vector<std::string> splitText (const std::string & source) {
      std::vector<std::string> results;
      for (std::size_t index = 0; index < source.size(); ++index) {
        std::size_t start = index;
        while (source[index] != '\n' && index < source.size()) {
          ++index;
        }
        
        results.emplace_back(source.substr(start, index - start + 1));
      }
      
      // Account for potential trailing newline.
      if (results.back().back() == '\n') {
        results.emplace_back("");
      }

      return results;
    }
  }
  
  Document::Document () {
  }
  
  Document::Document (const std::string & content)
  : m_rows(splitText(content)) {
  }
  
  std::string Document::contents () const {
    std::ostringstream stream;
    for (const std::string & text : m_rows) {
      stream << text;
    }
    
    return stream.str();
  }
  
  DocumentIterator Document::begin () const {
    return DocumentIterator(*this, Location(0, 0));
  }
  
  DocumentIterator Document::end () const {
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
    std::vector<std::string> lines = splitText(text);
    ReverseSelectionSetIterator cursor = selections.rbegin();
    while(cursor != selections.rend()) {
      Location lower = cursor->lowerBound();
      std::string prefix = m_rows[lower.row()].substr(0, lower.column());
      std::string suffix = m_rows[lower.row()].substr(lower.column());
      std::size_t insertRow = lower.row();
      m_rows[insertRow] = prefix + lines[0];
      
      for (std::size_t lineIndex = 1; lineIndex < lines.size(); ++lineIndex) {
        m_rows.insert(m_rows.begin() + insertRow + 1, lines[lineIndex]);
        ++insertRow;
      }
      
      std::size_t column = m_rows[insertRow].length();
      m_rows[insertRow] += suffix;
      
      Location origin(column, insertRow);
      cursor->setOrigin(origin);
      cursor->setExtent(origin);
      
      ++cursor;
    }
  }
  
  void Document::erase (SelectionSet & selections) {
    ReverseSelectionSetIterator cursor = selections.rbegin();
    while(cursor != selections.rend()) {
      Location lower = cursor->lowerBound();
      Location upper = cursor->upperBound();
      std::uint64_t modified = cursor->height();
      
      std::string prefix = m_rows[lower.row()].substr(0, lower.column());
      std::string suffix;
      if (upper.column() == m_rows[upper.row()].length() - 1) {
        suffix = m_rows[upper.row() + 1];
        ++modified;
      } else {
        suffix = m_rows[upper.row()].substr(upper.column() + 1);
      }
      
      std::string result = prefix + suffix;
      while (modified > 1) {
        m_rows.erase(m_rows.begin() + lower.row() + 1);
        --modified;
      }

      m_rows[lower.row()] = result;

      cursor->setExtent(cursor->origin());
      ++cursor;
    }
  }
    
  SelectionSet Document::matches (const SearchExpression & expression) const {
    typedef std::regex_iterator<DocumentIterator, char> RegexIterator;
    
    std::vector<Selection> results;
    if (expression.valid()) {
      RegexIterator cursor(begin(), end(), expression.pattern(), std::regex_constants::match_not_null);
      RegexIterator end;
      
      while (cursor != end) {
        auto match = *cursor;
        if (match.length() == 0) {
          // https://llvm.org/bugs/show_bug.cgi?id=21597 notes that libc++ doesn't currently
          // respect the "ignore empty matches" flag passed above. This can cause partially-entered
          // expressions containing \b assertions to generate an infinite loop, since incrementing
          // the iterator will never actually advance it. As a workaround, matches for a row are
          // aborted if any are empty, since that should only occur in the context of the libc++ bug.
          break;
        }
        
        for (std::size_t matchIndex = 0; matchIndex < match.size(); ++matchIndex) {
          Location origin = match[matchIndex].first.location();
          Location extent = std::prev(match[matchIndex].second).location();
          results.emplace_back(origin, extent);
        }
        
        ++cursor;
      }
    }
    
    return SelectionSet(results);
  }
}