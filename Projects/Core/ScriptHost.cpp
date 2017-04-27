#include "ScriptHost.hpp"

#include "AttributeRange.hpp"
#include "Script.hpp"

#include <iostream>

namespace quip {
  ScriptHost::ScriptHost(const std::string& rootPath)
  : m_lua(luaL_newstate())
  , m_root(rootPath) {
    luaL_openlibs(m_lua);
    addScriptPackagePath(rootPath);
    
    // Create the global Quip object.
    lua_newtable(m_lua);
    
    // Store the quip object globally.
    lua_setglobal(m_lua, "quip");
  }
  
  ScriptHost::~ScriptHost() {
    lua_close(m_lua);
  }
  
  const std::string& ScriptHost::scriptRootPath() const {
    return m_root;
  }
  
  Script ScriptHost::getScript(const std::string& path) {
    lua_getglobal(m_lua, path.c_str());
    if (lua_isnil(m_lua, -1)) {
      lua_pop(m_lua, 1);
      
      int result = luaL_loadfile(m_lua, path.c_str());
      if (result != 0) {
        std::cerr << lua_tostring(m_lua, -1) << std::endl;
      } else {
        lua_setglobal(m_lua, path.c_str());
      }
    }
    
    return Script(path);
  }
  
  void ScriptHost::runScript(const Script& script) {
    lua_getglobal(m_lua, script.identifier().c_str());
    if (lua_isnil(m_lua, -1)) {
      lua_pop(m_lua, 1);
      std::cerr << "Script not found.\n";
    }
    else {
      int result = lua_pcall(m_lua, 0, LUA_MULTRET, 0);
      if (result != 0) {
        std::cerr << lua_tostring(m_lua, -1);
      }
    }
  }
  
  std::vector<AttributeRange> ScriptHost::parseSyntax(const Script& script, const std::string& text) {
    std::vector<AttributeRange> results;

    // Recover the function from the global table and push it into the stack.
    lua_getglobal(m_lua, script.identifier().c_str());
    if (lua_isnil(m_lua, -1)) {
      lua_pop(m_lua, 1);
      std::cerr << "Script not found.\n";
    }
    else {
      // Push the function's arguments and call the function.
      lua_pushstring(m_lua, text.c_str());
      int result = lua_pcall(m_lua, 1, LUA_MULTRET, 0);
      if (result != 0) {
        std::cerr << lua_tostring(m_lua, -1);
      } else {
        // The result of a syntax script is a table containing the matches.
        // The table will contain sets of three items: the attribute group, the first character and the last character.
        if (lua_istable(m_lua, -1)) {
          std::size_t count = lua_rawlen(m_lua, -1);
          for (std::size_t item = 0; item < count; item += 3) {
            lua_geti(m_lua, -1, item + 1);
            std::string name = lua_tostring(m_lua, -1);
            lua_pop(m_lua, 1);

            lua_geti(m_lua, -1, item + 2);
            std::size_t start = lua_tointeger(m_lua, -1);
            --start;
            lua_pop(m_lua, 1);
            
            lua_geti(m_lua, -1, item + 3);
            std::size_t length = lua_tointeger(m_lua, -1);
            --length;
            lua_pop(m_lua, 1);
            
            // Length is actually a position at this point, so adjust it.
            length -= start;
            results.emplace_back(name, start, length);
          }
          
          // Pop the table.
          lua_pop(m_lua, 1);
        }
        
      }
    }
    
    return results;
  }
  
  void ScriptHost::addScriptPackagePath(const std::string& path) {
    addPackagePath("path", path + "/?.lua");
  }
  
  void ScriptHost::addNativePackagePath(const std::string& path) {
    addPackagePath("cpath", path + "/?.so");
  }
  
  void ScriptHost::addPackagePath(const std::string& variable, const std::string& path) {
    lua_getglobal(m_lua, "package");
    lua_getfield(m_lua, -1, variable.c_str());
    
    std::string current = lua_tostring(m_lua, -1);
    current += ";" + path;
    
    // Pop the old string, push the new one.
    lua_pop(m_lua, 1);
    lua_pushstring(m_lua, current.c_str());
    lua_setfield(m_lua, -2, variable.c_str());
    lua_pop(m_lua, 1);
  }
}
