#include "World.hpp"
#include "../EngineContext.hpp"

void Lua::World::registerLua(lua_State *L) {
    lua_getglobal(L, "Engine");
    luaL_newmetatable(L, kLuaStaticMetaTable);
    const struct luaL_Reg funcs[] = {
        {"loadMap", loadMapS},
        {nullptr, nullptr}
    };
    luaL_setfuncs(L, funcs, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_setfield(L, -2, "World");
    lua_pop(L, 1);
}

int Lua::World::loadMapS(lua_State *L) noexcept {
    const char *map_name = luaL_checkstring(L, 1);
    EngineContext::core()->loadMap(map_name);
    return 0;
}
