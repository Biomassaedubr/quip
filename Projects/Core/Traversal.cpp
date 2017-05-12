#include "Traversal.hpp"

#include "Document.hpp"

namespace quip {
  Traversal Traversal::documentOrder(const Document& document) {
    return Traversal(&nextInDocumentOrder, document.end(), &priorInDocumentOrder, document.begin());
  }
  
  Traversal Traversal::reverseDocumentOrder(const Document& document) {
    return Traversal(priorInDocumentOrder, document.begin(), nextInDocumentOrder, document.end());
  }
}
