#include "Classification.hpp"

#include "Document.hpp"
#include "DocumentIterator.hpp"
#include "Selection.hpp"

namespace quip {
  Selection classifyWord (const Document & document, const Location & location, ClassificationFlags flags) {
    if (flags & ClassificationFlags::Next) {
      DocumentIterator cursor = document.at(location);
      DocumentIterator end = document.end();
      
      while(cursor != end && !std::isspace(*cursor)) {
        ++cursor;
      }
      
      while(cursor != end && std::isspace(*cursor)) {
        ++cursor;
      }
      
      Location origin = cursor.location();
      while(cursor != end && !std::isspace(*cursor)) {
        ++cursor;
      }
      
      return Selection(origin, cursor.location());
    } else if (flags & ClassificationFlags::Prior) {
      DocumentIterator cursor = document.at(location);
      DocumentIterator begin = document.begin();
      
      while(cursor != begin && !std::isspace(*cursor)) {
        --cursor;
      }
      
      while(cursor != begin && std::isspace(*cursor)) {
        --cursor;
      }
      
      Location origin = cursor.location();
      while(cursor != begin && !std::isspace(*cursor)) {
        --cursor;
      }
      
      return Selection(cursor.location(), origin);
    } else {
      DocumentIterator cursor = document.at(location);
      DocumentIterator begin = document.begin();
      DocumentIterator end = document.end();

      if (std::isspace(*cursor)) {
        // If the cursor is on whitespace, move forward to the first non-whitespace.
        while(cursor != end && std::isspace(*cursor)) {
          ++cursor;
        }
      } else {
        // If the cursor is a on a non-whitespace character, move backward to the first whitespace.
        while(cursor != begin && !std::isspace(*cursor)) {
          --cursor;
        }
        
        // Land on the first actual character in the word.
        if (cursor != begin && std::isspace(*cursor)) {
          ++cursor;
        }
      }
            
      // Then move to the end of the word.
      Location origin = cursor.location();
      while(cursor != end && !std::isspace(*cursor)) {
        ++cursor;
      }
      
      while(cursor != end && std::isspace(*cursor)) {
        ++cursor;
      }
      
      --cursor;
      return Selection(origin, cursor.location());
    }
  }
  
  Selection classifyLine (const Document & document, const Location & location, ClassificationFlags flags) {
    if (document.isEmpty()) {
      return Selection(Location(0, 0));
    }
    
    if (flags & ClassificationFlags::Next) {
      std::size_t row = location.row() + 1 < document.rows() ? location.row() + 1 : document.rows() - 1;
      Location origin(0, row);
      Location extent(document.row(row).size() - 1, row);
      return Selection(origin, extent);
    } else if (flags & ClassificationFlags::Prior) {
      std::size_t row = location.row() > 0 ? location.row() - 1 : 0;
      Location origin(0, row);
      Location extent(document.row(row).size() - 1, row);
      return Selection(origin, extent);
    } else {
      Location origin(0, location.row());
      Location extent(document.row(location.row()).size() - 1, location.row());
      return Selection(origin, extent);
    }
  }
}