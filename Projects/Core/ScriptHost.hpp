#pragma once

#include "AttributeRange.hpp"
#include "Lua.hpp"
#include "ScriptBoundObject.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace quip {
  struct Script;
  
  struct ScriptHost {
    ScriptHost(const std::string& rootPath);
    ~ScriptHost();
    
    const std::string& scriptRootPath() const;
    
    Script getScript(const std::string& path);
    void runScript(const Script& script);
    
    std::vector<AttributeRange> parseSyntax(const Script& script, const std::string& text);
    
    void addScriptPackagePath(const std::string& path);
    void addNativePackagePath(const std::string& path);
    
    template<typename ObjectType>
    void bind(ObjectType* object, const std::string& name) {
      m_objects.emplace_back(std::make_unique<ScriptBoundObject>(object, name, ObjectType::binding(), m_lua));
    }
    
    ScriptHost(const ScriptHost& other) = delete;
    ScriptHost(ScriptHost&& other) = delete;
    ScriptHost& operator=(const ScriptHost& other) = delete;
    ScriptHost& operator=(ScriptHost&& other) = delete;
    
  private:
    lua_State* m_lua;
    std::string m_root;
    std::unordered_map<std::string, Script> m_cache;
    
    std::vector<std::unique_ptr<ScriptBoundObject>> m_objects;
    
    void addPackagePath(const std::string& variable, const std::string& path);
  };
}
