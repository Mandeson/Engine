#pragma once

#include <lua.hpp>

namespace Lua::Keyboard {
constexpr const char *kLuaStaticMetaTable = "KeyboardStatic";

void registerLua(lua_State *L);
int isKeyPressedS(lua_State *L) noexcept;
}
