#include "Syntax.hpp"

#include "MarkdownGrammar.hpp"
#include "UnknownGrammar.hpp"

namespace quip {
  namespace Syntax {
    SyntaxParseFunction getSyntaxForExtention (const std::string & extension) {
      if (extension == "md" || extension == "markdown") {
        return MarkdownGrammar::parse;
      }
      
      return getSyntaxForUnknown();
    }
  
    SyntaxParseFunction getSyntaxForUnknown() {
      return UnknownGrammar::parse;
    }
  }
}