#pragma once

#include <lua.hpp>

namespace Lua::Font {
constexpr const char *kLuaStaticMetaTable = "FontStatic";
constexpr const char *kLuaMetaTable = "Font";

void registerLua(lua_State *L);
int loadS(lua_State *L) noexcept;
int getDefaultS(lua_State *L) noexcept;
int __gc(lua_State *L) noexcept;
}
