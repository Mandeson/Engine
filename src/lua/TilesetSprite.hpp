#pragma once

#include <lua.hpp>

namespace Lua::TilesetSprite {
constexpr const char *kLuaStaticMetaTable = "TilesetSpriteStatic";
constexpr const char *kLuaMetaTable = "TilesetSprite";

void registerLua(lua_State *L);
int newS(lua_State *L) noexcept;
int setPos(lua_State *L) noexcept;
int setDepth(lua_State *L) noexcept;
int getPos(lua_State *L) noexcept;
int move(lua_State *L) noexcept;
int __gc(lua_State *L) noexcept;
}
