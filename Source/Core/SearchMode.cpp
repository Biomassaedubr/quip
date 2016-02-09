#include "SearchMode.hpp"

#include "Document.hpp"
#include "EditContext.hpp"
#include "KeyStroke.hpp"
#include "SearchExpression.hpp"

namespace quip {
  SearchMode::SearchMode () {
    addMapping(Key::Escape, &SearchMode::commitSearch);
    addMapping(Key::Return, &SearchMode::commitSearch);
  }
  
  std::string SearchMode::status () const {
    return "s/" + m_search;
  }
  
  bool SearchMode::onUnmappedKey (const KeyStroke & keyStroke, EditContext & context) {
    if (keyStroke.text().size() == 0 && keyStroke.key() != Key::Delete) {
      return false;
    }
    
    if (keyStroke.key() == Key::Delete) {
      if (m_search.size() > 0) {
        m_search.pop_back();
      }
    } else {
      m_search += keyStroke.text();
    }
    
    if (m_search.size() > 0) {
      SearchExpression expression(m_search);
      if (expression.valid()) {
        SelectionSet matches = context.document().matches(expression);
        context.selections().replace(matches);
      }
    }
    
    return true;
  }
  
  void SearchMode::commitSearch (EditContext & context) {
    m_search = "";
    
    context.leaveMode();
  }
}