#include "InsertTransaction.hpp"

#include "Document.hpp"
#include "EditContext.hpp"

namespace quip {
  InsertTransaction::InsertTransaction(const SelectionSet& selections, const std::vector<std::string>& text)
  : m_selections(selections)
  , m_text(text) {
  }
  
  InsertTransaction::~InsertTransaction() {
  }
  
  void InsertTransaction::perform(EditContext& context) {
    context.selections().replace(context.document().insert(m_selections, m_text));
  }
  
  void InsertTransaction::rollback(EditContext& context) {
    context.selections().replace(context.document().erase(m_selections));
  }
  
  std::shared_ptr<Transaction> InsertTransaction::create(const SelectionSet& selections, const std::string& text) {
    return std::make_shared<InsertTransaction>(selections, std::vector<std::string> {selections.count(), text });
  }
  
  std::shared_ptr<Transaction> InsertTransaction::create(const SelectionSet& selections, const std::vector<std::string>& text) {
    return std::make_shared<InsertTransaction>(selections, text);
  }
}
