#pragma once

#include <regex>
#include <string>

namespace quip {
  struct SearchExpression {
    SearchExpression (const std::string & expression);
    
    bool valid () const;
    
    const std::string & expression () const;
    const std::regex & pattern () const;
    
  private:
    bool m_valid;
    std::string m_expression;
    std::regex m_pattern;
  };
}