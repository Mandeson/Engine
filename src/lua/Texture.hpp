#pragma once

#include <lua.hpp>

namespace Lua::Texture {
constexpr const char *kLuaStaticMetaTable = "TextureStatic";
constexpr const char *kLuaMetaTable = "Texture";

void registerLua(lua_State *L);
int loadS(lua_State *L);
int __gc(lua_State *L);
}
