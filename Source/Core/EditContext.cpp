#include "EditContext.hpp"

#include "Document.hpp"
#include "EditMode.hpp"
#include "Location.hpp"
#include "Mode.hpp"
#include "NormalMode.hpp"
#include "SearchMode.hpp"
#include "Selection.hpp"

#include <memory>

namespace quip {
  EditContext::EditContext ()
  : EditContext(std::make_shared<Document>()) {
  }
  
  EditContext::EditContext (std::shared_ptr<Document> document)
  : m_document(document)
  , m_selections(Selection(Location(0, 0))) {
    
    // Populate with standard modes.
    m_modes.insert(std::make_pair("EditMode", std::make_shared<EditMode>()));
    m_modes.insert(std::make_pair("NormalMode", std::make_shared<NormalMode>()));
    m_modes.insert(std::make_pair("SearchMode", std::make_shared<SearchMode>()));
    
    enterMode("NormalMode");
  }
  
  Document & EditContext::document () {
    return *m_document;
  }
  
  SelectionSet & EditContext::selections () {
    return m_selections;
  }
  
  Mode & EditContext::mode () {
    return *m_modeHistory.top();
  }
  
  void EditContext::enterMode (const std::string & name) {
    std::map<std::string, std::shared_ptr<Mode>>::iterator cursor = m_modes.find(name);
    if (cursor != m_modes.end()) {
      m_modeHistory.push(cursor->second);
    }
  }
  
  void EditContext::leaveMode () {
    if (m_modeHistory.size() > 1) {
      m_modeHistory.pop();
    }
  }
  
  bool EditContext::processKey (const KeyStroke & keyStroke) {
    return mode().processKey(keyStroke, *this);
  }
}