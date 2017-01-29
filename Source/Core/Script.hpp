#pragma once

#include "Lua.hpp"

#include <string> 

namespace quip {
  struct Script {
    Script(const std::string& path);
    ~Script();
    
    void run();
    
    void addPackagePath(const std::string& path);
    
  private:
    lua_State* m_state;
    std::string m_path;
  };
}
