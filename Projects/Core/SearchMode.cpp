#include "SearchMode.hpp"

#include "Color.hpp"
#include "Document.hpp"
#include "EditContext.hpp"
#include "SearchExpression.hpp"
#include "SelectionDrawInfo.hpp"

namespace quip {
  SearchMode::SearchMode() {
    addMapping(Key::Escape, &SearchMode::abortSearch);
    addMapping(Key::Return, &SearchMode::commitSearch);
  }
  
  std::string SearchMode::status() const {
    return "s/" + m_search;
  }
  
  bool SearchMode::onUnmappedKey(Key key, const std::string& text, EditContext& context) {
    if (text.size() == 0 && key != Key::Delete) {
      return false;
    }
    
    if (key == Key::Delete) {
      if (m_search.size() > 0) {
        m_search.pop_back();
      }
    } else {
      m_search += text;
    }
    
    if (m_search.size() > 0) {
      SearchExpression expression(m_search);
      if (expression.valid()) {
        SelectionDrawInfo overlay;
        overlay.selections = context.document().matches(expression);
        overlay.flags = CursorFlags::None;
        overlay.style = CursorStyle::VerticalBlock;
        overlay.primaryColor = Color(1.0f, 1.0f, 0.2f);
        overlay.secondaryColor = Color(1.0f, 1.0f, 0.8f);
        context.setOverlay("Search", overlay);
      }
    }
    
    return true;
  }
  
  void SearchMode::abortSearch(EditContext& context) {
    m_search = "";
    
    context.clearOverlay("Search");
    context.leaveMode();
  }
  
  void SearchMode::commitSearch(EditContext& context) {
    context.selections().replace(context.document().matches(SearchExpression(m_search)));
    m_search = "";

    context.clearOverlay("Search");
    context.leaveMode();
  }
}
