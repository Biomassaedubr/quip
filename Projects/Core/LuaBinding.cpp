#include "LuaBinding.hpp"

#include <iostream>

namespace quip {
  bool LuaBinding::hasFunction(const std::string& name) const {
    return m_functions.find(name) != std::end(m_functions);
  }
  
  void LuaBinding::invokeFunction(const std::string& name, void* object, lua_State* state) const {
    auto iterator = m_functions.find(name);
    if (iterator != std::end(m_functions)) {
      iterator->second(object, state);
    } else {
      std::cerr << "object has no function '" << name << "'\n";
    }
  }
  
  void LuaBinding::invokePropertyGetter(const std::string& name, void* object, lua_State* state) const {
    auto iterator = m_getters.find(name);
    if (iterator != std::end(m_getters)) {
      iterator->second(object, state);
    } else {
      std::cerr << "object has no getter for '" << name << "'\n";
    }
  }
  
  void LuaBinding::invokePropertySetter(const std::string& name, void* object, lua_State* state) const {
    auto iterator = m_setters.find(name);
    if (iterator != std::end(m_setters)) {
      iterator->second(object, state);
    } else {
      std::cerr << "object has no setter for '" << name << "'\n";
    }
  }
  
  void LuaBinding::pushValue(lua_State* state, const std::string& value) {
    lua_pushstring(state, value.c_str());
  }
  
  void LuaBinding::pushValue(lua_State* state, float value) {
    lua_pushnumber(state, value);
  }
  
  void LuaBinding::popValue(lua_State* state, std::string* value) {
    *value = lua_tostring(state, -1);
    lua_pop(state, 1);
  }
  
  void LuaBinding::popValue(lua_State* state, float* value) {
    *value = lua_tonumber(state, -1);
    lua_pop(state, 1);
  }
  
  void LuaBinding::popValues(lua_State* state, std::tuple<>& values) {
  }
}
