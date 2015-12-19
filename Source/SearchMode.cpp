#include "SearchMode.hpp"

#include "Document.hpp"
#include "EditContext.hpp"
#include "KeyStroke.hpp"

namespace quip {
  SearchMode::SearchMode () {
    addMapping(Key::Escape, &SearchMode::commitSearch);
    addMapping(Key::Return, &SearchMode::commitSearch);
  }
  
  std::string SearchMode::status () const {
    return "s/" + m_search;
  }
  
  bool SearchMode::onUnmappedKey (const KeyStroke & keyStroke, EditContext & context) {
    if (keyStroke.text().size() > 0) {
      m_search += keyStroke.text();
      
      SelectionSet matches = context.document().matches(m_search);
      context.selections().replace(matches);
      
      return true;
    }
    
    return false;
  }
  
  void SearchMode::commitSearch (EditContext & context) {
    m_search = "";
    
    context.leaveMode();
  }
}