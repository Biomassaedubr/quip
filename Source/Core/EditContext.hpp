#pragma once

#include "Key.hpp"
#include "KeyStroke.hpp"
#include "SelectionDrawInfo.hpp"
#include "SelectionSet.hpp"
#include "ViewController.hpp"

#include <map>
#include <memory>
#include <stack>
#include <string>

namespace quip {
  struct Document;
  struct Mode;
  
  struct EditContext {
    EditContext ();
    EditContext (std::shared_ptr<Document> document);
    
    Document & document ();
    SelectionSet & selections ();
    Mode & mode ();

    const std::map<std::string, SelectionDrawInfo> & overlays () const;
    void setOverlay (const std::string & name, const SelectionDrawInfo & overlay);
    void clearOverlay (const std::string & name);
    
    void enterMode (const std::string & name);
    void leaveMode ();
    
    bool processKey (const KeyStroke & keyStroke);
    
    ViewController & controller ();
    
  private:
    std::shared_ptr<Document> m_document;
    
    SelectionSet m_selections;
    std::map<std::string, SelectionDrawInfo> m_overlays;
    
    std::map<std::string, std::shared_ptr<Mode>> m_modes;
    std::stack<std::shared_ptr<Mode>> m_modeHistory;
    
    ViewController m_controller;
  };
}