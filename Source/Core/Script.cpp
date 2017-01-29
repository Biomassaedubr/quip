#include "Script.hpp"

#include <iostream>

namespace quip {
  Script::Script(const std::string& identifier)
  : m_identifier(identifier) {
  }
  
  const std::string& Script::identifier() const {
    return m_identifier;
  }
}
