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
  
  SelectionSet Document::insert (const SelectionSet & selections, const std::string & text) {
    if (selections.count() == 0 || text.size() == 0) {
      return selections;
    }
    
    // The updated selection set cannot be larger than the initial selection set, so storage
    // can be reserved up front.
    std::vector<Selection> updated;
    updated.reserve(selections.count());
    
    // Similarly, the upper bound on the amount of document rows required is the product of the
    // selection count and number of new lines to be inserted; attempting to reserve the appropriate
    // storage in the document up front will amortize the cost of reallocating (and moving) the
    // entire row array.
    std::vector<std::string> lines = splitText(text);
    std::size_t rowsInserted = lines.size() - 1;
    m_rows.reserve(m_rows.size() + (selections.count() * rowsInserted));

    // As text is inserted, it may cause the document to shift underneath subsequent selections.
    // Those shifts must be tracked in order to ensure the inserted text is placed correctly.
    std::int64_t columnShift = 0;
    std::int64_t rowShift = 0;
    std::size_t priorUnmodifiedRow = 0;
    for (const Selection & selection : selections) {
      Location lowerBound = selection.lowerBound();
      
      // Column shift only applies to selections that occupy the same (unmodified) row.
      // It gets reset when moving to a new unmodified row.
      std::uint64_t unmodifiedRow = lowerBound.row();
      if (priorUnmodifiedRow != unmodifiedRow) {
        columnShift = 0;
        priorUnmodifiedRow = unmodifiedRow;
      }
      
      Location insertionPoint = lowerBound.adjustBy(columnShift, rowShift);
      std::uint64_t insertionColumn = insertionPoint.column();
      std::uint64_t insertionRow = insertionPoint.row();
      std::string prefix = m_rows[insertionRow].substr(0, insertionColumn);
      std::string suffix = m_rows[insertionRow].substr(insertionColumn);
      m_rows[insertionRow] = prefix + lines.front();
      
      if (rowsInserted > 0) {
        // Make room for additional lines in the document storage.
        m_rows.insert(m_rows.begin() + insertionRow + 1, rowsInserted, "");
        for (std::size_t line = 1; line < lines.size(); ++line) {
          ++insertionRow;
          m_rows[insertionRow] = lines[line];
        }
        
        columnShift -= prefix.length();
      } else {
        columnShift += lines.front().size();
      }
      
      // Complete the insert.
      m_rows[insertionRow] += suffix;
      
      Location location(m_rows[insertionRow].size() - suffix.size(), insertionRow);
      updated.emplace_back(location, location);
      
      // Row shift is applied for every selection.
      rowShift += rowsInserted;
    }
    
    return SelectionSet(updated);
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