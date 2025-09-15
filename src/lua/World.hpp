#pragma once

#include <lua.hpp>

namespace Lua::World {
constexpr const char *kLuaStaticMetaTable = "WorldStatic";

void registerLua(lua_State *L);
int loadMapS(lua_State *L) noexcept;
}
