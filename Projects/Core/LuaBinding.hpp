#pragma once

#include "Lua.hpp"

#include <experimental/tuple>
#include <string>
#include <unordered_map>

namespace quip {
  struct LuaBinding {
    bool hasFunction(const std::string& name) const;
    
    template<typename ObjectType, typename ReturnType, typename... Arguments>
    void addFunction(const std::string& name, ReturnType (ObjectType::*function)(Arguments...)) {
      m_functions[name] = [this, function](auto object, auto state) {
        ObjectType* target = reinterpret_cast<ObjectType*>(object);
        std::tuple<Arguments...> arguments;
        popValues(state, arguments);
        
        // Before applying, stick the member pointer in front of the parameter tuple (it will
        // become the 'this' pointer).
        std::experimental::apply(function, std::tuple_cat(std::make_tuple(target), arguments));
      };
    }
    
    template<typename ObjectType, typename PropertyType>
    void addProperty(const std::string& name, PropertyType (ObjectType::*getter)() const, void (ObjectType::*setter)(PropertyType)) {
      m_getters[name] = [this, getter] (void* object, lua_State* state) {
        ObjectType* target = reinterpret_cast<ObjectType*>(object);
        PropertyType result = (target->*getter)();
        pushValue(state, result);
      };
      
      m_setters[name] = [this, setter] (void* object, lua_State* state) {
        typename std::remove_const<typename std::remove_reference<PropertyType>::type>::type value;
        popValue(state, &value);
        
        ObjectType* target = reinterpret_cast<ObjectType*>(object);
        (target->*setter)(value);
      };
    }
    
    void invokeFunction(const std::string& name, void* object, lua_State* state) const;
    void invokePropertyGetter(const std::string& name, void* object, lua_State* state) const;
    void invokePropertySetter(const std::string& name, void* object, lua_State* state) const;
    
  private:
    std::unordered_map<std::string, std::function<void (void*, lua_State*)>> m_getters;
    std::unordered_map<std::string, std::function<void (void*, lua_State*)>> m_setters;
    std::unordered_map<std::string, std::function<void (void*, lua_State*)>> m_functions;
    
    void pushValue(lua_State* state, const std::string& value);
    void pushValue(lua_State* state, float value);
    
    void popValue(lua_State* state, std::string* value);
    void popValue(lua_State* state, float* value);
    void popValues(lua_State* state, std::tuple<>& values);
    
    template<typename FirstArgument, typename... RemainingArguments>
    void popValues(lua_State* state, std::tuple<FirstArgument, RemainingArguments...>& values) {
      FirstArgument first;
      popValue(state, &first);
      
      std::tuple<RemainingArguments...> remaining;
      popValues(state, remaining);
      
      values = std::tuple_cat(remaining, std::make_tuple(first));
    }
  };
}
