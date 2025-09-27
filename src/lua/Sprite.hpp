#pragma once

#include <lua.hpp>

namespace Lua::Sprite {
constexpr const char *kLuaStaticMetaTable = "SpriteStatic";
constexpr const char *kLuaMetaTable = "Sprite";

void registerLua(lua_State *L);
int newS(lua_State *L) noexcept;
int setPos(lua_State *L) noexcept;
int getPos(lua_State *L) noexcept;
int move(lua_State *L) noexcept;
int __gc(lua_State *L) noexcept;
}
