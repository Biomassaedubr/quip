#include "Syntax.hpp"

#include <regex>

namespace quip {
  Syntax::Syntax () {
    m_keywords.emplace_back("auto");
    m_keywords.emplace_back("bool");
    m_keywords.emplace_back("break");
    m_keywords.emplace_back("case");
    m_keywords.emplace_back("catch");
    m_keywords.emplace_back("char");
    m_keywords.emplace_back("class");
    m_keywords.emplace_back("const");
    m_keywords.emplace_back("const_cast");
    m_keywords.emplace_back("constexpr");
    m_keywords.emplace_back("continue");
    m_keywords.emplace_back("decltype");
    m_keywords.emplace_back("do");
    m_keywords.emplace_back("double");
    m_keywords.emplace_back("dynamic_cast");
    m_keywords.emplace_back("else");
    m_keywords.emplace_back("enum");
    m_keywords.emplace_back("explicit");
    m_keywords.emplace_back("extern");
    m_keywords.emplace_back("false");
    m_keywords.emplace_back("float");
    m_keywords.emplace_back("for");
    m_keywords.emplace_back("friend");
    m_keywords.emplace_back("goto");
    m_keywords.emplace_back("if");
    m_keywords.emplace_back("inline");
    m_keywords.emplace_back("int");
    m_keywords.emplace_back("long");
    m_keywords.emplace_back("mutable");
    m_keywords.emplace_back("namespace");
    m_keywords.emplace_back("new");
    m_keywords.emplace_back("noexcept");
    m_keywords.emplace_back("nullptr");
    m_keywords.emplace_back("operator");
    m_keywords.emplace_back("private");
    m_keywords.emplace_back("protected");
    m_keywords.emplace_back("public");
    m_keywords.emplace_back("register");
    m_keywords.emplace_back("reinterpret_cast");
    m_keywords.emplace_back("return");
    m_keywords.emplace_back("short");
    m_keywords.emplace_back("signed");
    m_keywords.emplace_back("sizeof");
    m_keywords.emplace_back("static");
    m_keywords.emplace_back("static_assert");
    m_keywords.emplace_back("static_cast");
    m_keywords.emplace_back("struct");
    m_keywords.emplace_back("switch");
    m_keywords.emplace_back("template");
    m_keywords.emplace_back("this");
    m_keywords.emplace_back("throw");
    m_keywords.emplace_back("true");
    m_keywords.emplace_back("try");
    m_keywords.emplace_back("typedef");
    m_keywords.emplace_back("typeid");
    m_keywords.emplace_back("typename");
    m_keywords.emplace_back("union");
    m_keywords.emplace_back("unsigned");
    m_keywords.emplace_back("using");
    m_keywords.emplace_back("virtual");
    m_keywords.emplace_back("void");
    m_keywords.emplace_back("volatile");
    m_keywords.emplace_back("while");
    
    std::string pattern;
    for (std::size_t index = 0; index < m_keywords.size(); ++index) {
      pattern += "\\b" + m_keywords[index] + "\\b";
      if (index + 1 < m_keywords.size()) {
        pattern += "|";
      }
    }
    
    m_keywordPattern = std::regex(pattern);
  }
  
  std::vector<AttributeRange> Syntax::highlight (const std::string & text) const {
    std::vector<AttributeRange> result;
    std::sregex_iterator cursor(text.begin(), text.end(), m_keywordPattern);
    std::sregex_iterator end;
    while (cursor != end) {
      AttributeRange range(Keyword, cursor->position(), cursor->length());
      result.push_back(range);
      ++cursor;
    }
    
    return result;
  }
}