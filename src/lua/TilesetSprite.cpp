#include "TilesetSprite.hpp"
#include "../util/Logger.hpp"
#include "../EngineContext.hpp"
#include <cstdint>

void Lua::TilesetSprite::registerLua(lua_State *L) {
    luaL_newmetatable(L, kLuaStaticMetaTable);
    const struct luaL_Reg funcs[] = {
        {"new", newS},
        //{"getDefault", getDefaultS},
        {nullptr, nullptr}
    };
    luaL_setfuncs(L, funcs, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_setfield(L, -2, "TilesetSprite");
}

int Lua::TilesetSprite::newS(lua_State *L) noexcept {
    const char *tileset_name = luaL_checkstring(L, 1);
    Vector2<uint16_t> texture_pos = Vector2{luaL_checkinteger(L, 2), luaL_checkinteger(L, 3)};
    Vector2i size = Vector2{luaL_checkinteger(L, 4), luaL_checkinteger(L, 5)};
    auto sprite_id_ptr = reinterpret_cast<TilesetSpriteId *>(lua_newuserdata(L, sizeof(TilesetSpriteId)));
    try {
        auto &tileset = EngineContext::core()->getTilesetManager().getTileset(tileset_name);
        *sprite_id_ptr = EngineContext::core()->getTilesetSpriteManager().newObject(tileset, texture_pos, size);
    } catch (std::exception &e) {
        lua_pop(L, 1);
        lua_pushnil(L);
        luaL_error(L, e.what());
        return 1;
    }
    Log::info("tileset sprite newS");
    if (luaL_newmetatable(L, kLuaMetaTable)) {
        const struct luaL_Reg methods[] = {
            {"setPos", setPos},
            {"getPos", getPos},
            {"move", move},
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

int Lua::TilesetSprite::setPos(lua_State *L) noexcept {
    auto sprite_id = *reinterpret_cast<TilesetSpriteId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    Vector2d pos = Vector2{luaL_checknumber(L, 2), luaL_checknumber(L, 3)};
    EngineContext::core()->getTilesetSpriteManager().setPos(sprite_id, pos);
    return 0;
}

int Lua::TilesetSprite::getPos(lua_State *L) noexcept {
    auto sprite_id = *reinterpret_cast<TilesetSpriteId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    Vector2d pos = EngineContext::core()->getTilesetSpriteManager().getPos(sprite_id);
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int Lua::TilesetSprite::move(lua_State *L) noexcept {
    auto sprite_id = *reinterpret_cast<TilesetSpriteId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    Vector2d move = Vector2{luaL_checknumber(L, 2), luaL_checknumber(L, 3)};
    EngineContext::core()->getTilesetSpriteManager().move(sprite_id, move);
    return 0;
}

int Lua::TilesetSprite::__gc(lua_State *L) noexcept {
    Log::info("tileset sprite __gc");
    auto sprite_id_ptr = reinterpret_cast<TilesetSpriteId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    EngineContext::core()->getTilesetSpriteManager().destroyObject(*sprite_id_ptr);
    *sprite_id_ptr = -1;
    return 0;
}