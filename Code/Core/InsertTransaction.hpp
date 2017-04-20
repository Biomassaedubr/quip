#pragma once

#include "SelectionSet.hpp"
#include "Transaction.hpp"

#include <string>
#include <memory>
#include <vector>

namespace quip {
  struct SelectionSet;
  
  struct InsertTransaction : Transaction {
    InsertTransaction (const SelectionSet & selections, const std::vector<std::string> & text);
    ~InsertTransaction ();
    
    void perform (EditContext & context) override;
    void rollback (EditContext & context) override;
    
    static std::shared_ptr<Transaction> create (const SelectionSet & selections, const std::string & text);
    static std::shared_ptr<Transaction> create (const SelectionSet & selections, const std::vector<std::string> & text);
    
  private:
    SelectionSet m_selections;
    std::vector<std::string> m_text;
  };
}