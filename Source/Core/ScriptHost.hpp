#pragma once

#include "Lua.hpp"

#include <string>
#include <unordered_map>

namespace quip {
  struct Script;
  
  struct ScriptHost {
    ScriptHost();
    ~ScriptHost();
    
    Script getScript(const std::string& path);
    void runScript(const Script& script);
    
    void addNativePackagePath(const std::string& path);
    
    ScriptHost(const ScriptHost& other) = delete;
    ScriptHost(ScriptHost&& other) = delete;
    ScriptHost& operator=(const ScriptHost& other) = delete;
    ScriptHost& operator=(ScriptHost&& other) = delete;
    
  private:
    lua_State* m_lua;
    std::unordered_map<std::string, Script> m_cache;
  };
}
