#include "Document.hpp"

#include "DocumentIterator.hpp"
#include "SearchExpression.hpp"
#include "Selection.hpp"
#include "SelectionSet.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>

namespace quip {
  namespace {
    std::vector<std::string> splitText (const std::string & source) {
      std::vector<std::string> results;
      if (source.size() == 0) {
        results.emplace_back("");
        return results;
      }
      
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
  
  Document::Document ()
  : m_syntaxParseFunction(Syntax::getSyntaxForUnknown()) {
  }
  
  Document::Document (const std::string & content)
  : m_syntaxParseFunction(Syntax::getSyntaxForUnknown())
  , m_rows(splitText(content)) {
  }
  
  std::string Document::contents () const {
    std::ostringstream stream;
    for (const std::string & text : m_rows) {
      stream << text;
    }
    
    return stream.str();
  }
  
  std::string Document::contents (const Selection & selection) const {
    Location lowerBound = selection.lowerBound();
    Location upperBound = selection.upperBound();
    
    if (selection.height() == 1) {
      return m_rows[lowerBound.row()].substr(lowerBound.column(), upperBound.column() - lowerBound.column() + 1);
    } else {
      std::string result = m_rows[lowerBound.row()].substr(lowerBound.column());
      for (std::size_t index = 1; index < selection.height() - 1; ++index) {
        result += m_rows[lowerBound.row() + index];
      }
      
      result += m_rows[upperBound.row()].substr(0, upperBound.column() + 1);
      return result;
    }
  }
  
  std::vector<std::string> Document::contents (const SelectionSet & selections) const {
    std::vector<std::string> results;
    results.reserve(selections.count());
    
    for (const Selection & selection : selections) {
      results.emplace_back(contents(selection));
    }
    
    return results;
  }
  
  DocumentIterator Document::begin () const {
    return DocumentIterator(*this, Location(0, 0));
  }
  
  DocumentIterator Document::end () const {
    return DocumentIterator(*this, Location(0, m_rows.size()));
  }
  
  DocumentIterator Document::at (const Location & location) const {
    return DocumentIterator(*this, location);
  }
  
  const std::string & Document::path () const {
    return m_path;
  }
  
  void Document::setPath (const std::string & path) {
    m_path = path;
    
    // Find the extension.
    std::size_t index = m_path.find_last_of('.');
    if (index != std::string::npos) {
      m_syntaxParseFunction = Syntax::getSyntaxForExtention(m_path.substr(index + 1));
    }
  }
  
  const std::string & Document::row (std::size_t index) const {
    return m_rows[index];
  }
  
  std::size_t Document::rows () const {
    return m_rows.size();
  }
  
  Location Document::clip (const Location & location) const {
    std::size_t row = std::min(location.row(), static_cast<std::uint64_t>(m_rows.size()) - 1);
    std::size_t column = std::min(location.column(), static_cast<std::uint64_t>(m_rows[row].size()) - 1);
    
    return Location(column, row);
  }
  
  SelectionSet Document::insert (const SelectionSet & selections, const std::string & text) {
    std::vector<std::string> replicated(selections.count(), text);
    return insert(selections, replicated);
  }

  
  SelectionSet Document::insert (const SelectionSet & selections, const std::vector<std::string> & text) {
    if (selections.count() == 0 || text.size() == 0) {
      return selections;
    }
    
    // The updated selection set cannot be larger than the initial selection set, so storage
    // can be reserved up front.
    std::vector<Selection> updated;
    updated.reserve(selections.count());
    
    // As text is inserted, it may cause the document to shift underneath subsequent selections.
    // Those shifts must be tracked in order to ensure the inserted text is placed correctly.
    std::int64_t columnShift = 0;
    std::int64_t rowShift = 0;
    for (std::size_t index = 0; index < selections.count(); ++index) {
      std::vector<std::string> lines = splitText(text[index]);
      std::size_t rowsInserted = lines.size() - 1;
      
      const Selection & selection = selections[index];
      Location insertionPoint = selection.lowerBound().adjustBy(columnShift, rowShift);
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
      
      // Row shift is applied continually, but the column shift gets reset
      // when moving to selections that originated on a different row.
      if (index + 1 < selections.count()) {
        rowShift += rowsInserted;
        
        const Selection & next = selections[index + 1];
        if (selection.lowerBound().row() != next.lowerBound().row()) {
          columnShift = 0;
        }
      }
      
      // Record where the selection should move post-insert.
      Location location(m_rows[insertionRow].size() - suffix.size(), insertionRow);
      updated.emplace_back(location, location);
    }
    
    return SelectionSet(updated);
  }
  
  SelectionSet Document::erase (const SelectionSet & selections) {
    if (selections.count() == 0) {
      return selections;
    }
    
    // The updated selection set cannot be larger than the initial selection set, so storage
    // can be reserved up front.
    std::vector<Selection> updated;
    updated.reserve(selections.count());
    
    // As text is removed, it may cause the document to shift underneath subsequent selections.
    // Those shifts must be tracked in order to ensure all the correct text is erased.
    std::int64_t columnShift = 0;
    std::int64_t rowShift = 0;
    for (std::size_t index = 0; index < selections.count(); ++index) {
      const Selection & selection = selections[index];
      Location lowerBound = selection.lowerBound().adjustBy(columnShift, rowShift);
      Location upperBound = selection.upperBound().adjustBy(columnShift, rowShift);
      std::size_t rowsRemoved = selection.height() - 1;
      
      // The prefix is easy to compute, but the suffix must be special-cased when a line
      // terminator or the very last character in the document is removed.
      std::string prefix = m_rows[lowerBound.row()].substr(0, lowerBound.column());
      std::string suffix;
      
      const std::string & upperLine = m_rows[upperBound.row()];
      if (upperBound.column() == upperLine.length() - 1) {
        if (upperLine.back() == '\n') {
          suffix = m_rows[upperBound.row() + 1];
          ++rowsRemoved;
        } else {
          // Only the last character in the document can (legally) terminate a row without
          // being a newline.
          suffix = "";
          --columnShift;
        }
      } else {
        suffix = m_rows[upperBound.row()].substr(upperBound.column() + 1);
      }
      
      std::size_t lowerLineLength = m_rows[lowerBound.row()].size();
      m_rows.erase(m_rows.begin() + lowerBound.row() + 1, m_rows.begin() + lowerBound.row() + rowsRemoved + 1);
      m_rows[lowerBound.row()] = prefix + suffix;
      
      // Clip the selection to the document bounds.
      updated.push_back(clip(selection.origin().adjustBy(columnShift, rowShift)));
      columnShift -= lowerLineLength - m_rows[lowerBound.row()].size();
      
      
      if (index + 1 < selections.count()) {
        const Selection & next = selections[index + 1];
        
        if(selection.lowerBound().row() == next.lowerBound().row() - 1) {
          columnShift = -(lowerLineLength - m_rows[lowerBound.row()].size());
          rowShift -= rowsRemoved;
          
        } else if (selection.lowerBound().row() != next.lowerBound().row()) {
          columnShift = 0;
          rowShift -= rowsRemoved;
        }
      }
    }
    
    return SelectionSet(updated);
  }
  
  SelectionSet Document::matches (const SearchExpression & expression) const {
    std::vector<Selection> results;
    if (expression.valid()) {
      std::string content;
      std::vector<std::size_t> table = buildSpanTable(&content);
      std::sregex_iterator cursor(content.begin(), content.end(), expression.pattern(), std::regex_constants::match_not_null);
      std::sregex_iterator end;
      
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
          Location origin = linearPositionToLocation(table, cursor->position());
          Location extent = linearPositionToLocation(table, cursor->position() + cursor->length() - 1);
          results.emplace_back(origin, extent);
        }
        
        ++cursor;
      }
    }
    
    return SelectionSet(results);
  }
  
  std::vector<AttributeRange> Document::highlight (std::uint64_t row) const {
    return m_syntaxParseFunction(m_rows[row], m_path);
  }
  
  std::vector<std::size_t> Document::buildSpanTable (std::string * contents) const {
    // A "span table" accelerates the process of finding a column/row location
    // from a linear index into the document.
    std::vector<std::size_t> table;
    table.reserve(m_rows.size());
    
    std::ostringstream stream;
    std::size_t length = 0;
    for (const std::string & text : m_rows) {
      if (contents != nullptr) {
        stream << text;
      }
      
      length += text.size();
      table.emplace_back(length);
    }
    
    if (contents != nullptr) {
      *contents = stream.str();
    }
    
    return table;
  }
  
  Location Document::linearPositionToLocation(const std::vector<std::size_t> & spanTable, std::size_t position) const {
    std::vector<std::size_t>::const_iterator span = std::lower_bound(spanTable.begin(), spanTable.end(), position);
    auto row = span - spanTable.begin();
    if (position == *span) {
      // If the position precisely matches a span entry, it's actually the next
      // row that's interesting.
      ++row;
      ++span;
    }
    
    std::size_t column = m_rows[row].size() - (*span - position);
    return Location(column, row);
  }
}
