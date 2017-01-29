#include "Script.hpp"

#include <iostream>

namespace quip {
  Script::Script(const std::string& path)
  : m_state(luaL_newstate())
  , m_path(path) {
    luaL_openlibs(m_state);
  }
  
  Script::~Script() {
    lua_close(m_state);
  }
  
  void Script::run() {
    int result = luaL_loadfile(m_state, m_path.c_str());
    result = lua_pcall(m_state, 0, LUA_MULTRET, 0);
    if (result == LUA_ERRRUN) {
      std::cerr << lua_tostring(m_state, -1) << std::endl;
    }
  }
  
  void Script::addPackagePath(const std::string& path) {
    lua_getglobal(m_state, "package");
    lua_getfield(m_state, -1, "cpath");
    
    std::string current = lua_tostring(m_state, -1);
    current += ";" + path + "/?.so";
    
    // Pop the old string, push the new one.
    lua_pop(m_state, 1);
    lua_pushstring(m_state, current.c_str());
    lua_setfield(m_state, -2, "cpath");
    lua_pop(m_state, 1);
  }
}
