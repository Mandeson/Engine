#pragma once

#include <lua.hpp>

namespace Lua::Window {
constexpr const char *kLuaStaticMetaTable = "WindowStatic";

void registerLua(lua_State *L);
int getSizeS(lua_State *L) noexcept;
}
