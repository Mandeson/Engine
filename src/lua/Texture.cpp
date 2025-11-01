#include "Texture.hpp"
#include "../Texture.hpp"
#include "../EngineContext.hpp"

void Lua::Texture::registerLua(lua_State *L) {
    luaL_newmetatable(L, kLuaStaticMetaTable);
    const struct luaL_Reg funcs[] = {
        {"load", loadS},
        {nullptr, nullptr}
    };
    luaL_setfuncs(L, funcs, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_setfield(L, -2, "Texture");
}

int Lua::Texture::loadS(lua_State *L) {
    const char *texture_name = luaL_checkstring(L, 1);
    auto texture_ptr = reinterpret_cast<::Texture **>(lua_newuserdata(L, sizeof(::Texture *)));

    // The Texture constructor doesn't throw any exceptions
    *texture_ptr = new ::Texture(EngineContext::core()->getThreadPool(), texture_name);

    if (luaL_newmetatable(L, kLuaMetaTable)) {
        const struct luaL_Reg methods[] = {
            {"__gc", __gc},
            {nullptr, nullptr}
        };
        luaL_setfuncs(L, methods, 0);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
    }
    lua_setmetatable(L, -2);
    return 1;
}

int Lua::Texture::__gc(lua_State *L) {
    auto *ptr = reinterpret_cast<::Texture **>(luaL_checkudata(L, 1, kLuaMetaTable));
    delete *ptr;
    *ptr = nullptr;
    return 0;
}
