#pragma once

#include <lua.hpp>

namespace Lua::Text {
constexpr const char *kLuaStaticMetaTable = "TextStatic";
constexpr const char *kLuaMetaTable = "Text";

void registerLua(lua_State *L);
int newS(lua_State *L) noexcept;
int setString(lua_State *L) noexcept;
int setScale(lua_State *L) noexcept;
int setPos(lua_State *L) noexcept;
int setColor(lua_State *L) noexcept;
int __gc(lua_State *L) noexcept;
}
