#include "ScriptHost.hpp"

#include "Script.hpp"

#include <iostream>

namespace quip {
  ScriptHost::ScriptHost()
  : m_lua(luaL_newstate()) {
    luaL_openlibs(m_lua);
  }
  
  ScriptHost::~ScriptHost() {
    lua_close(m_lua);
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
  
  void ScriptHost::addNativePackagePath(const std::string& path) {
    lua_getglobal(m_lua, "package");
    lua_getfield(m_lua, -1, "cpath");
    
    std::string current = lua_tostring(m_lua, -1);
    current += ";" + path + "/?.so";
    
    // Pop the old string, push the new one.
    lua_pop(m_lua, 1);
    lua_pushstring(m_lua, current.c_str());
    lua_setfield(m_lua, -2, "cpath");
    lua_pop(m_lua, 1);
  }
}
