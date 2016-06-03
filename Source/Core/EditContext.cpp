#include "EditContext.hpp"

#include "Document.hpp"
#include "EditMode.hpp"
#include "Location.hpp"
#include "Mode.hpp"
#include "NormalMode.hpp"
#include "SearchMode.hpp"
#include "Selection.hpp"
#include "Transaction.hpp"

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
  
  const std::map<std::string, SelectionDrawInfo> & EditContext::overlays () const {
    return m_overlays;
  }
  
  void EditContext::setOverlay (const std::string & name, const SelectionDrawInfo & overlay) {
    m_overlays[name] = overlay;
  }
  
  void EditContext::clearOverlay (const std::string & name) {
    auto cursor = m_overlays.find(name);
    if (cursor != std::end(m_overlays)) {
      m_overlays.erase(cursor);
    }
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
  
  void EditContext::performTransaction(std::shared_ptr<Transaction> transaction) {
    transaction->perform(*this);
    m_undoStack.push(transaction);
  }
  
  void EditContext::undo () {
    if (m_undoStack.size() > 0) {
      m_undoStack.top()->rollback(*this);
      m_redoStack.push(m_undoStack.top());
      
      m_undoStack.pop();
    }
  }
  
  void EditContext::redo () {
    if (m_redoStack.size() > 0) {
      m_redoStack.top()->perform(*this);
      m_undoStack.push(m_redoStack.top());

      m_redoStack.pop();
    }
  }
  
  bool EditContext::processKey (const KeyStroke & keyStroke) {
    return mode().processKey(keyStroke, *this);
  }
  
  ViewController & EditContext::controller () {
    return m_controller;
  }
}