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
}
