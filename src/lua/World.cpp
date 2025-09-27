#include "World.hpp"
#include "../EngineContext.hpp"
#include <exception>
#include <stdexcept>

void Lua::World::registerLua(lua_State *L) {
    luaL_newmetatable(L, kLuaStaticMetaTable);
    const struct luaL_Reg funcs[] = {
        {"loadMap", loadMapS},
        {"setCameraPos", setCameraPosS},
        {"getCameraPos", getCameraPosS},
        {"getScreenTileSize", getScreenTileSizeS},
        {nullptr, nullptr}
    };
    luaL_setfuncs(L, funcs, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_setfield(L, -2, "World");
}

int Lua::World::loadMapS(lua_State *L) noexcept {
    const char *map_name = luaL_checkstring(L, 1);
    EngineContext::core()->loadMap(map_name);
    return 0;
}

int Lua::World::setCameraPosS(lua_State *L) noexcept {
    Vector2d pos = Vector2{luaL_checknumber(L, 1), luaL_checknumber(L, 2)};
    EngineContext::core()->setCameraPos(pos);
    return 0;
}

int Lua::World::getCameraPosS(lua_State *L) noexcept {
    Vector2d pos = EngineContext::core()->getCameraPos();
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int Lua::World::getScreenTileSizeS(lua_State *L) noexcept {
    try {
        auto &map = EngineContext::core()->getMap();
        if (!map.ready())
            throw std::runtime_error("Attempted to call getScreenTileSize when the map still being loaded");
        int tile_size = map.getTileSize();
        int pixel_scale = EngineContext::game()->getWorldRenderer().getPixelScale();
        lua_pushinteger(L, tile_size * pixel_scale);
    } catch (std::exception &e) {
        lua_pushnil(L);
        luaL_error(L, e.what());
        return 1;
    }
    return 1;
}

