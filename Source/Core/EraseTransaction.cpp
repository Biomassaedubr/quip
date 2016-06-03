#include "EraseTransaction.hpp"

#include "Document.hpp"
#include "EditContext.hpp"

namespace quip {
  EraseTransaction::EraseTransaction (const SelectionSet & selections)
  : m_selections(selections) {
  }
  
  EraseTransaction::~EraseTransaction () {
  }
  
  void EraseTransaction::perform (EditContext & context) {
    // Capture the text covered by the selections prior to erasing it so that it
    // can be restored later.
    m_text = context.document().contents(m_selections);
    
    context.selections().replace(context.document().erase(m_selections));
  }
  
  void EraseTransaction::rollback (EditContext & context) {
    context.selections().replace(context.document().insert(m_selections, m_text));
  }
  
  std::shared_ptr<Transaction> EraseTransaction::create (const SelectionSet & selections) {
    return std::make_shared<EraseTransaction>(selections);
  }
}