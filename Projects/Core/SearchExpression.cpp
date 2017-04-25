#include "SearchExpression.hpp"

namespace quip {
  SearchExpression::SearchExpression(const std::string& expression)
  : m_valid(true)
  , m_expression(expression) {
    if (expression.length() == 0) {
      m_valid = false;
    } else if (expression[expression.length() - 1] == '\\') {
      // libc++ doesn't throw on trailing slashes like it should.
      // See: https://llvm.org/bugs/show_bug.cgi?id=26175
      m_valid = false;
    } else {
      try {
        m_pattern = std::regex(expression);
      } catch (const std::regex_error&) {
        m_valid = false;
      }
    }
  }
  
  bool SearchExpression::valid() const {
    return m_valid;
  }
  
  const std::string& SearchExpression::expression() const {
    return m_expression;
  }
  
  const std::regex& SearchExpression::pattern() const {
    return m_pattern;
  }
}
