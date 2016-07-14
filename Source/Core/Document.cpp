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
  Document::Document ()
  : m_syntaxParseFunction(Syntax::getSyntaxForUnknown()) {
  }
  
  Document::Document (const std::string & content)
  : m_syntaxParseFunction(Syntax::getSyntaxForUnknown())
  , m_rows(decompose(content)) {
  }
  
  std::string Document::contents () const {
    std::ostringstream stream;
    for (const std::string & text : m_rows) {
      stream << text;
    }
    
    return stream.str();
  }
  
  bool Document::isEmpty () const noexcept {
    return m_rows.size() == 0;
  }
  
  bool Document::isMissingTrailingNewline () const noexcept {
    if (m_rows.empty()) {
      // By definition.
      return true;
    }
    
    const std::string & text = m_rows.back();
    return !text.empty() && text.back() != '\n';
  }
  
  std::string Document::contents (const Selection & selection) const {
    if (m_rows.size() == 0) {
      // Selections always cover at least one character, so it's not
      // ambiguous to return an empty string for an empty document.
      // Although strictly speaking the only selection for which this
      // is legal and consistent is (0,0).
      return "";
    }
    
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
    if (m_rows.size() == 0) {
      return results;
    }
    
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
  
  std::int64_t Document::distance (const Location & from, const Location & to) const {
    if (from.row() == to.row()) {
      return to.column() - from.column();
    }
    
    Location lower = std::min(from, to);
    Location upper = std::max(from, to);
    std::int64_t result = m_rows[lower.row()].size() - lower.column();
    for (std::uint64_t index = lower.row() + 1; index < upper.row(); ++index) {
      result += m_rows[index].size();
    }
    
    result += upper.column();
    return from <= to ? result : -result;
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

  SelectionSet Document::insert (const Selection & selection, const std::string & text) {
    return insert(SelectionSet(selection), text);
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
    
    std::int64_t columnShift = 0;
    std::int64_t rowShift = 0;
    for (std::uint64_t index = 0; index < selections.count(); ++index) {
      std::vector<std::string> lines = decompose(text[index]);
      if (lines.size() == 0) {
        continue;
      }
      
      if (m_rows.size() == 0) {
        // If the document is empty, just copy the entire lines array in and break the loop.
        // The selection set is basically irrelevant; the only legal set for an empty document
        // consists entirely of the single-character selection at (0, 0).
        m_rows = lines;
        updated.emplace_back(Location(m_rows.back().size(), m_rows.size() - 1));
        break;
      }
      
      const Selection & selection = selections[index];
      Location origin = selection.origin().adjustBy(columnShift, rowShift);
      std::string prefix = m_rows[origin.row()].substr(0, origin.column());
      std::string suffix = m_rows[origin.row()].substr(origin.column());
      std::uint64_t rowsToInsert = lines.size() - 1;
      if (lines.back().back() == '\n') {
        ++rowsToInsert;
      }
      
      // Compose resulting text, inserting rows as needed.
      m_rows[origin.row()] = prefix + lines.front();
      m_rows.insert(m_rows.begin() + origin.row() + 1, rowsToInsert, "");
      for (std::uint64_t line = 1; line <= lines.size() - 1; ++line) {
        m_rows[origin.row() + line] = lines[line];
      }
      
      m_rows[origin.row() + rowsToInsert] += suffix;
      
      // Update shifts to track how this selection impacts any subsequent selections.
      columnShift += lines.front().size();
      rowShift += rowsToInsert;
      if (index + 1 < selections.count()) {
        const Selection & next = selections[index + 1];
        if (selection.extent().row() != next.origin().row()) {
          // Whether or not adjacent selections have the same line in common impacts how
          // to shift the next selection.
          columnShift = 0;
        }
      }
      
      // Insert operations displace selections such that the origin remains after the
      // text that was inserted.
      if (rowsToInsert == 0) {
        updated.emplace_back(Location(origin.column() + lines.front().size(), origin.row()));
      } else {
        std::uint64_t row = origin.row() + rowsToInsert;
        updated.emplace_back(Location(m_rows[row].size() - suffix.size(), row));
      }
    }
    
    return SelectionSet(updated);
  }
  
  SelectionSet Document::erase (const Selection & selection) {
    return erase(SelectionSet(selection));
  }
  
  SelectionSet Document::erase (const SelectionSet & selections) {
    if (selections.count() == 0) {
      return selections;
    }
    
    // The updated selection set cannot be larger than the initial selection set, so storage
    // can be reserved up front.
    std::vector<Selection> updated;
    updated.reserve(selections.count());

    std::int64_t columnShift = 0;
    std::int64_t rowShift = 0;
    for (std::uint64_t index = 0; index < selections.count(); ++index) {
      const Selection & selection = selections[index];
      Location origin = selection.origin().adjustBy(columnShift, rowShift);
      Location extent = selection.extent().adjustBy(selection.height() == 1 ? columnShift : 0, rowShift);
      std::int64_t rowsToRemove = extent.row() - origin.row();
      
      // Whether or not the selection covers the trailing newline of a row or
      // includes the last row in the document impacts how the erasure is handled.
      bool hasLastCharacterInRow = extent.column() == row(extent.row()).size() - 1;
      bool hasLastRowInDocument = extent.row() == m_rows.size() - 1;
      
      // Determine how many rows to remove and how to compose the resulting text.
      std::string prefix = m_rows[origin.row()].substr(0, origin.column());
      std::string suffix;
      if (hasLastCharacterInRow && !hasLastRowInDocument) {
        suffix = m_rows[extent.row() + 1];
        ++rowsToRemove;
      } else {
        suffix = m_rows[extent.row()].substr(extent.column() + 1);
      }

      // Update shifts to track how this selection impacts any subsequent selections.
      rowShift -= rowsToRemove;
      if (rowsToRemove > 0) {
        columnShift = 0;
      }
      
      if (index + 1 < selections.count()) {
        const Selection & next = selections[index + 1];
        
        // Whether or not adjacent selections have the same line in common or are only
        // separated by a newline that will be removed impacts how to shift the
        // next selection.
        bool hasSameLine = selection.extent().row() == next.origin().row();
        bool hasNextLine = selection.extent().row() == next.origin().row() - 1;
        if (hasSameLine) {
          columnShift -= extent.column() - origin.column() + 1;
        } else if (hasNextLine && hasLastCharacterInRow) {
          columnShift = extent.column();
        } else {
          columnShift = 0;
        }
      }
      
      // Remove rows and write composed text.
      std::vector<std::string>::iterator firstRemovedRow = m_rows.begin() + origin.row();
      std::vector<std::string>::iterator lastRemovedRow = firstRemovedRow + rowsToRemove;
      m_rows.erase(firstRemovedRow, lastRemovedRow);
      m_rows[origin.row()] = prefix + suffix;
      
      // Erase operations collapse selections to the origin, generally. However, it's
      // possible that the origin no longer exists.
      if (origin <= Location(m_rows.back().size() - 1, m_rows.size() - 1)) {
        updated.emplace_back(origin);
      } else if (origin.column() > 0) {
        updated.emplace_back(origin.adjustBy(-1, 0));
      } else if (origin.row() > 0) {
        std::uint64_t row = origin.row() - 1;
        updated.emplace_back(Location(m_rows[row].size() - 1, row));
      } else {
        updated.emplace_back(Location(0, 0));
      }
    }
    
    // If the very last character of the document was removed, also remove the
    // very last row so that the document's internal text state is consistent with
    // a default-constructed, empty document.
    if (m_rows.size() == 1 && m_rows.back().size() == 0) {
      m_rows.clear();
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
  
  std::vector<std::string> Document::decompose (const std::string & text) const {
    std::vector<std::string> results;
    if (text.size() == 0) {
      return results;
    }
    
    std::string::size_type start = 0;
    std::string::size_type end = 0;
    while (start < text.size()) {
      end = text.find_first_of('\n', start);
      if (end != std::string::npos) {
        results.emplace_back(text.substr(start, end - start + 1));
        start = end + 1;
      } else {
        results.emplace_back(text.substr(start));
        break;
      }
    }
    
    return results;
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
