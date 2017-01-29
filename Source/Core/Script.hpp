#pragma once

#include "Lua.hpp"

#include <string> 

namespace quip {
  struct Script {
    Script();
    Script(const std::string& identifier);
    
    const std::string& identifier() const;
    
  private:
    std::string m_identifier;
  };
}
