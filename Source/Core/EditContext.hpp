#pragma once

#include "Key.hpp"
#include "KeyStroke.hpp"
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

    void enterMode (const std::string & name);
    void leaveMode ();
    
    bool processKey (const KeyStroke & keyStroke);
    
    ViewController & controller ();
    
  private:
    std::shared_ptr<Document> m_document;
    SelectionSet m_selections;

    std::map<std::string, std::shared_ptr<Mode>> m_modes;
    std::stack<std::shared_ptr<Mode>> m_modeHistory;
    
    ViewController m_controller;
  };
}