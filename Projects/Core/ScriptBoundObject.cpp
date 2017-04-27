#include "ScriptBoundObject.hpp"

namespace quip {
  namespace {
    int callMemberFunction(lua_State* state) {
      ScriptBoundObject* object = reinterpret_cast<ScriptBoundObject*>(lua_touserdata(state, lua_upvalueindex(1)));
      std::string member = lua_tostring(state, lua_upvalueindex(2));
      object->callFunction(member, state);
      
      return 0;
    }
    
    int getBoundObjectMember(lua_State* state) {
      std::string member = lua_tostring(state, -1);
      ScriptBoundObject** slot = reinterpret_cast<ScriptBoundObject**>(lua_touserdata(state, -2));
      ScriptBoundObject* object = *slot;

      if (object->binding().hasFunction(member)) {
        // If the member is a function, push a closure allowing the eventual call of that function
        // from Lua to refer back to this specific bound object.
        lua_pushlightuserdata(state, object);
        lua_pushstring(state, member.c_str());
        lua_pushcclosure(state, callMemberFunction, 2);
      } else {
        // Otherwise, assume the member is a property getter and invoke that directly.
        ScriptBoundObject** slot = reinterpret_cast<ScriptBoundObject**>(lua_touserdata(state, -2));
        ScriptBoundObject* binding = *slot;
        binding->getPropertyValue(member, state);
      }
      
      return 1;
    }
    
    int setBoundObjectMember(lua_State* state) {
      std::string member = lua_tostring(state, -2);
      ScriptBoundObject** slot = reinterpret_cast<ScriptBoundObject**>(lua_touserdata(state, -3));
      ScriptBoundObject* binding = *slot;
      binding->setPropertyValue(member, state);
      
      return 0;
    }
    
    luaL_Reg BoundObjectMetatable[] = {
      { "__index", getBoundObjectMember },
      { "__newindex", setBoundObjectMember },
      { nullptr, nullptr }
    };
  }
  
  ScriptBoundObject::ScriptBoundObject(void* object, const std::string& name, const LuaBinding& binding, lua_State* state)
  : m_object(object)
  , m_binding(binding) {
  
    // All bound objects are attached to the Quip global table.
    lua_getglobal(state, "quip");
    
    // The pointer to the object is stored in a heavy userdata (so that it can have a metatable).
    ScriptBoundObject** slot = reinterpret_cast<ScriptBoundObject**>(lua_newuserdata(state, sizeof(ScriptBoundObject*)));
    *slot = this;

    // Create the metatable to hook indexing into the bound object from Lua.
    lua_newtable(state);
    luaL_setfuncs(state, BoundObjectMetatable, 0);
    lua_setmetatable(state, -2);
    
    // Insert the bound object's userdata into the Quip global table with the specified name.
    lua_setfield(state, -2, name.c_str());
  }
  
  const LuaBinding& ScriptBoundObject::binding() const {
    return m_binding;
  }
  
  void ScriptBoundObject::callFunction(const std::string& name, lua_State* state) {
    m_binding.invokeFunction(name, m_object, state);
  }
  
  void ScriptBoundObject::getPropertyValue(const std::string& key, lua_State* state) {
    m_binding.invokePropertyGetter(key, m_object, state);
  }
  
  void ScriptBoundObject::setPropertyValue(const std::string& key, lua_State* state) {
    m_binding.invokePropertySetter(key, m_object, state);
  }
}
