#include "EditContext.hpp"

#include "Document.hpp"
#include "InsertMode.hpp"
#include "Location.hpp"
#include "Mode.hpp"
#include "NavigationMode.hpp"
#include "Selection.hpp"

#include <memory>

namespace quip {
  EditContext::EditContext ()
  : EditContext(std::make_shared<Document>()) {
  }
  
  EditContext::EditContext (std::shared_ptr<Document> document)
  : m_document(document)
  , m_selections(Selection(Location(0, 0), Location(0, 0))) {
    
    // Populate with standard modes.
    m_modes.insert(std::make_pair("InsertMode", std::make_shared<InsertMode>()));
    m_modes.insert(std::make_pair("NavigationMode", std::make_shared<NavigationMode>()));
    
    enterMode("NavigationMode");
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