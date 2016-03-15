#include "Syntax.hpp"

#include <regex>

namespace quip {
  namespace {
    Syntax gFallbackSyntax("?", {
      { Keyword, "\\bQuip\\b" },
      { Preprocessor, "\\bWelcome\\b" },
    });
                           
    Syntax gCppSyntax("C++", {
      { Keyword, "\\bauto\\b" },
      { Keyword, "\\bbool\\b" },
      { Keyword, "\\bbreak\\b" },
      { Keyword, "\\bcase\\b" },
      { Keyword, "\\bcatch\\b" },
      { Keyword, "\\bchar\\b" },
      { Keyword, "\\bclass\\b" },
      { Keyword, "\\bconst\\b" },
      { Keyword, "\\bconst_cast\\b" },
      { Keyword, "\\bconstexpr\\b" },
      { Keyword, "\\bcontinue\\b" },
      { Keyword, "\\bdecltype\\b" },
      { Keyword, "\\bdo\\b" },
      { Keyword, "\\bdouble\\b" },
      { Keyword, "\\bdynamic_cast\\b" },
      { Keyword, "\\belse\\b" },
      { Keyword, "\\benum\\b" },
      { Keyword, "\\bexplicit\\b" },
      { Keyword, "\\bextern\\b" },
      { Keyword, "\\bfalse\\b" },
      { Keyword, "\\bfloat\\b" },
      { Keyword, "\\bfor\\b" },
      { Keyword, "\\bfriend\\b" },
      { Keyword, "\\bgoto\\b" },
      { Keyword, "\\bif\\b" },
      { Keyword, "\\binline\\b" },
      { Keyword, "\\bint\\b" },
      { Keyword, "\\blong\\b" },
      { Keyword, "\\bmutable\\b" },
      { Keyword, "\\bnamespace\\b" },
      { Keyword, "\\bnew\\b" },
      { Keyword, "\\bnoexcept\\b" },
      { Keyword, "\\bnullptr\\b" },
      { Keyword, "\\boperator\\b" },
      { Keyword, "\\bprivate\\b" },
      { Keyword, "\\bprotected\\b" },
      { Keyword, "\\bpublic\\b" },
      { Keyword, "\\bregister\\b" },
      { Keyword, "\\breinterpret_cast\\b" },
      { Keyword, "\\breturn\\b" },
      { Keyword, "\\bshort\\b" },
      { Keyword, "\\bsigned\\b" },
      { Keyword, "\\bsizeof\\b" },
      { Keyword, "\\bstatic\\b" },
      { Keyword, "\\bstatic_assert\\b" },
      { Keyword, "\\bstatic_cast\\b" },
      { Keyword, "\\bstruct\\b" },
      { Keyword, "\\bswitch\\b" },
      { Keyword, "\\btemplate\\b" },
      { Keyword, "\\bthis\\b" },
      { Keyword, "\\bthrow\\b" },
      { Keyword, "\\btrue\\b" },
      { Keyword, "\\btry\\b" },
      { Keyword, "\\btypedef\\b" },
      { Keyword, "\\btypeid\\b" },
      { Keyword, "\\btypename\\b" },
      { Keyword, "\\bunion\\b" },
      { Keyword, "\\bunsigned\\b" },
      { Keyword, "\\busing\\b" },
      { Keyword, "\\bvirtual\\b" },
      { Keyword, "\\bvoid\\b" },
      { Keyword, "\\bvolatile\\b" },
      { Keyword, "\\bwhile\\b" },
      { Preprocessor, "#.*" },
      { Comment, "//.*" },
    });
    
    Syntax gMarkdownSyntax("Markdown", {
      { Keyword, "#+" },
    });
  }
  
  Syntax::Syntax (const std::string & name, std::initializer_list<SyntaxPattern> patterns)
  : m_name(name)
  , m_patterns(patterns) {
  }
  
  const std::string & Syntax::name () const {
    return m_name;
  }
  
  std::vector<AttributeRange> Syntax::highlight (const std::string & text) const {
    std::vector<AttributeRange> result;
    
    for (const SyntaxPattern & pattern : m_patterns) {
      std::regex expression(pattern.pattern);
      std::sregex_iterator cursor(text.begin(), text.end(), expression);
      std::sregex_iterator end;
      while (cursor != end) {
        AttributeRange range(pattern.attribute, cursor->position(), cursor->length());
        result.push_back(range);
        ++cursor;
      }
    }
    
    return result;
  }
  
  Syntax * Syntax::getSyntaxForExtention (const std::string & extension) {
    if (extension == "cpp" || extension == "cxx") {
      return &gCppSyntax;
    } else if (extension == "md" || extension == "markdown") {
      return &gMarkdownSyntax;
    }
    
    return getSyntaxForUnknown();
  }
  
  Syntax * Syntax::getSyntaxForUnknown() {
    return &gFallbackSyntax;
  }
}