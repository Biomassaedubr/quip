#pragma once

#include "AttributeRange.hpp"
#include "Lua.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace quip {
  struct GlobalSettings;
  struct Script;
  
  struct ScriptHost {
    ScriptHost(const std::string& rootPath, GlobalSettings& settings);
    ~ScriptHost();
    
    const std::string& scriptRootPath() const;
    
    Script getScript(const std::string& path);
    void runScript(const Script& script);
    
    std::vector<AttributeRange> parseSyntax(const Script& script, const std::string& text);
    
    void addScriptPackagePath(const std::string& path);
    void addNativePackagePath(const std::string& path);
    
    ScriptHost(const ScriptHost& other) = delete;
    ScriptHost(ScriptHost&& other) = delete;
    ScriptHost& operator=(const ScriptHost& other) = delete;
    ScriptHost& operator=(ScriptHost&& other) = delete;
    
  private:
    lua_State* m_lua;
    std::string m_root;
    std::unordered_map<std::string, Script> m_cache;
    
    void addPackagePath(const std::string& variable, const std::string& path);
  };
}
