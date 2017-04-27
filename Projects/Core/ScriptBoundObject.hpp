#pragma once

#include "Lua.hpp"
#include "LuaBinding.hpp"

#include <functional>
#include <string>
#include <experimental/tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace quip {
  struct ScriptBoundObject {
    ScriptBoundObject(void* object, const std::string& name, const LuaBinding& type, lua_State* state);

    const LuaBinding& binding() const;
    
    void callFunction(const std::string& name, lua_State* state);
    void getPropertyValue(const std::string& key, lua_State* state);
    void setPropertyValue(const std::string& key, lua_State* state);

  private:
    void* m_object;
    LuaBinding m_binding;
  };
}
