#include "EditContext.hpp"

#include "Document.hpp"
#include "EditMode.hpp"
#include "JumpMode.hpp"
#include "Location.hpp"
#include "MarkdownSyntax.hpp"
#include "Mode.hpp"
#include "NormalMode.hpp"
#include "SearchMode.hpp"
#include "Selection.hpp"
#include "Transaction.hpp"
#include "UnknownSyntax.hpp"

#include <memory>

namespace quip {
  EditContext::EditContext (PopupService * popupService, StatusService * statusService)
  : EditContext(popupService, statusService, std::make_shared<Document>()) {
  }
  
  EditContext::EditContext (PopupService * popupService, StatusService * statusService, std::shared_ptr<Document> document)
  : m_document(document)
  , m_selections(Selection(Location(0, 0)))
  , m_popupService(popupService)
  , m_statusService(statusService) {
    
    // Populate with standard file types.
    m_fileTypeDatabase.registerFileType("Text", UnknownSyntax::get(), {"txt", "text"});
    m_fileTypeDatabase.registerFileType("Markdown", MarkdownSyntax::get(), {"md", "markdown"});
    m_fileTypeDatabase.registerFileType("C++ Source", UnknownSyntax::get(), {"cpp", "cxx"});
    m_fileTypeDatabase.registerFileType("C++ Header", UnknownSyntax::get(), {"hpp", "hxx"});
    m_fileTypeDatabase.registerFileType("GLSL Shader Source", UnknownSyntax::get(),  {"fsh", "vsh"});
    
    // Populate with standard modes.
    m_modes.insert(std::make_pair("EditMode", std::make_shared<EditMode>()));
    m_modes.insert(std::make_pair("JumpMode", std::make_shared<JumpMode>()));
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
      mode().enter(*this);
    }
  }
  
  void EditContext::leaveMode () {
    if (m_modeHistory.size() > 1) {
      mode().exit(*this);
      m_modeHistory.pop();
    }
  }
  
  void EditContext::performTransaction(std::shared_ptr<Transaction> transaction) {
    transaction->perform(*this);
    m_onTransactionApplied.transmit(ChangeType::Do);
    m_undoStack.push(transaction);
  }
  
  bool EditContext::canUndo() const noexcept {
    return m_undoStack.size() > 0;
  }
  
  void EditContext::undo () {
    if (canUndo()) {
      m_undoStack.top()->rollback(*this);
      m_onTransactionApplied.transmit(ChangeType::Undo);
      m_redoStack.push(m_undoStack.top());
      
      m_undoStack.pop();
    }
  }
  
  bool EditContext::canRedo() const noexcept {
    return m_redoStack.size() > 0;
  }
  
  void EditContext::redo () {
    if (canRedo()) {
      m_redoStack.top()->perform(*this);
      m_onTransactionApplied.transmit(ChangeType::Redo);
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
  
  PopupService & EditContext::popupService () {
    return *m_popupService;
  }
  
  StatusService & EditContext::statusService () {
    return *m_statusService;
  }
  
  const FileTypeDatabase & EditContext::fileTypeDatabase () const {
    return m_fileTypeDatabase;
  }
  
  Signal<void (ChangeType)> & EditContext::onTransactionApplied () {
    return m_onTransactionApplied;
  }
}