#include "Sprite.hpp"
#include "../util/Logger.hpp"
#include "../EngineContext.hpp"
#include "Texture.hpp"
#include <cstdint>
#include <lauxlib.h>

void Lua::Sprite::registerLua(lua_State *L) {
    luaL_newmetatable(L, kLuaStaticMetaTable);
    const struct luaL_Reg funcs[] = {
        {"new", newS},
        //{"getDefault", getDefaultS},
        {nullptr, nullptr}
    };
    luaL_setfuncs(L, funcs, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_setfield(L, -2, "Sprite");
}

int Lua::Sprite::newS(lua_State *L) noexcept {
    auto texture = *reinterpret_cast<::Texture **>(luaL_checkudata(L, 1, Lua::Texture::kLuaMetaTable));
    if (texture == nullptr) {
        lua_pop(L, 1);
        lua_pushnil(L);
        luaL_error(L, "Null texture pointer Engine.Sprite.new()");
        return 1;
    }

    Vector2<uint16_t> texture_pos = Vector2{luaL_checkinteger(L, 2), luaL_checkinteger(L, 3)};
    Vector2i size = Vector2{luaL_checkinteger(L, 4), luaL_checkinteger(L, 5)};

    auto sprite_id_ptr = reinterpret_cast<SpriteId *>(lua_newuserdata(L, sizeof(SpriteId)));
    try {
        *sprite_id_ptr = EngineContext::core()->getSpriteManager().newSprite(*texture, TextureRect{texture_pos, size});
    } catch (std::exception &e) {
        lua_pop(L, 1);
        lua_pushnil(L);
        luaL_error(L, e.what());
        return 1;
    }
    Log::info("sprite newS");
    if (luaL_newmetatable(L, kLuaMetaTable)) {
        const struct luaL_Reg methods[] = {
            {"setPos", setPos},
            {"setSize", setSize},
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

int Lua::Sprite::setPos(lua_State *L) noexcept {
    auto sprite_id = *reinterpret_cast<SpriteId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    Vector2d pos = Vector2{luaL_checknumber(L, 2), luaL_checknumber(L, 3)};
    EngineContext::core()->getSpriteManager().setPos(sprite_id, pos);
    return 0;
}

int Lua::Sprite::setSize(lua_State *L) noexcept {
    auto sprite_id = *reinterpret_cast<SpriteId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    Vector2i size = Vector2{luaL_checkinteger(L, 2), luaL_checkinteger(L, 3)};
    EngineContext::core()->getSpriteManager().setSize(sprite_id, size);
    return 0;
}

int Lua::Sprite::getPos(lua_State *L) noexcept {
    auto sprite_id = *reinterpret_cast<SpriteId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    Vector2d pos = EngineContext::core()->getSpriteManager().getPos(sprite_id);
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int Lua::Sprite::move(lua_State *L) noexcept {
    auto sprite_id = *reinterpret_cast<SpriteId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    Vector2d move = Vector2{luaL_checknumber(L, 2), luaL_checknumber(L, 3)};
    EngineContext::core()->getSpriteManager().move(sprite_id, move);
    return 0;
}

int Lua::Sprite::__gc(lua_State *L) noexcept {
    Log::info("sprite __gc");
    auto sprite_id_ptr = reinterpret_cast<SpriteId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    EngineContext::core()->getSpriteManager().destroySprite(*sprite_id_ptr);
    *sprite_id_ptr = -1;
    return 0;
}