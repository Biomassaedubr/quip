#include "AppendTransaction.hpp"

#include "Document.hpp"
#include "EditContext.hpp"

namespace quip {
  AppendTransaction::AppendTransaction (const SelectionSet & selections, const std::vector<std::string> & text)
  : m_selections(selections)
  , m_text(text) {
  }
  
  AppendTransaction::~AppendTransaction () {
  }
  
  void AppendTransaction::perform (EditContext & context) {
    m_rollbackSelections = context.document().append(m_selections, m_text);
    context.selections().replace(m_rollbackSelections);
  }
  
  void AppendTransaction::rollback (EditContext & context) {
    context.selections().replace(context.document().erase(m_rollbackSelections));
  }
  
  std::shared_ptr<Transaction> AppendTransaction::create (const SelectionSet & selections, const std::string & text) {
    return std::make_shared<AppendTransaction>(selections, std::vector<std::string> {selections.count(), text });
  }
  
  std::shared_ptr<Transaction> AppendTransaction::create (const SelectionSet & selections, const std::vector<std::string> & text) {
    return std::make_shared<AppendTransaction>(selections, text);
  }
}
