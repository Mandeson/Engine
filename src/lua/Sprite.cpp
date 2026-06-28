#include "Sprite.hpp"
#include <cstdint>
#include "../util/Logger.hpp"
#include "../EngineContext.hpp"
#include "Common.hpp"
#include "Texture.hpp"

struct SpriteLua {
    SpriteId sprite_id = -1;
    int texture_lua_ref = LUA_NOREF; // Lua reference to the associated Texture object
};

static void destroyedObjectWarn(lua_State *L, std::string_view method_name) {
    auto str = Lua::Common::methodCallOnDestroyedObjectErrorText("Sprite", method_name);
    luaL_error(L, str.c_str());
}

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

    auto sprite_lua_ptr = reinterpret_cast<SpriteLua *>(lua_newuserdata(L, sizeof(SpriteLua)));
    *sprite_lua_ptr = SpriteLua{}; // Initialize fields
    try {
        sprite_lua_ptr->sprite_id = EngineContext::core()->getSpriteManager().newObject(*texture, TextureRect{texture_pos, size});
    } catch (std::exception &e) {
        lua_pop(L, 1);
        lua_pushnil(L);
        luaL_error(L, e.what());
        return 1;
    }

    lua_pushvalue(L, 1);
    sprite_lua_ptr->texture_lua_ref = luaL_ref(L, LUA_REGISTRYINDEX);;

    Log::dbg("sprite newS");
    if (luaL_newmetatable(L, kLuaMetaTable)) {
        const struct luaL_Reg methods[] = {
            {"destroy", destroy},
            {"setVisible", setVisible},
            {"setPos", setPos},
            {"setSize", setSize},
            {"setDepth", setDepth},
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

int Lua::Sprite::destroy(lua_State *L) noexcept {
    auto sprite_lua_ptr = reinterpret_cast<SpriteLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (sprite_lua_ptr->sprite_id != -1) {
        EngineContext::core()->getSpriteManager().destroyObject(sprite_lua_ptr->sprite_id);
        sprite_lua_ptr->sprite_id = -1;
        if (sprite_lua_ptr->texture_lua_ref != LUA_NOREF) {
            luaL_unref(L, LUA_REGISTRYINDEX, sprite_lua_ptr->texture_lua_ref);
            sprite_lua_ptr->texture_lua_ref = LUA_NOREF;
        }
    }
    return 0;
}

int Lua::Sprite::setVisible(lua_State *L) noexcept {
    auto sprite_lua_ptr = reinterpret_cast<SpriteLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (sprite_lua_ptr->sprite_id == -1) {
        destroyedObjectWarn(L, "setVisible");
        return 0;
    }
    bool visible = static_cast<bool>(luaL_checkinteger(L, 2));
    EngineContext::core()->getSpriteManager().setVisible(sprite_lua_ptr->sprite_id, visible);
    return 0;
}

int Lua::Sprite::setPos(lua_State *L) noexcept {
    auto sprite_lua_ptr = reinterpret_cast<SpriteLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (sprite_lua_ptr->sprite_id == -1) {
        destroyedObjectWarn(L, "setPos");
        return 0;
    }
    Vector2d pos = Vector2{luaL_checknumber(L, 2), luaL_checknumber(L, 3)};
    EngineContext::core()->getSpriteManager().setPos(sprite_lua_ptr->sprite_id, pos);
    return 0;
}

int Lua::Sprite::setSize(lua_State *L) noexcept {
    auto sprite_lua_ptr = reinterpret_cast<SpriteLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (sprite_lua_ptr->sprite_id == -1) {
        destroyedObjectWarn(L, "setSize");
        return 0;
    }
    Vector2i size = Vector2{luaL_checkinteger(L, 2), luaL_checkinteger(L, 3)};
    EngineContext::core()->getSpriteManager().setSize(sprite_lua_ptr->sprite_id, size);
    return 0;
}

int Lua::Sprite::setDepth(lua_State *L) noexcept {
    auto sprite_lua_ptr = reinterpret_cast<SpriteLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (sprite_lua_ptr->sprite_id == -1) {
        destroyedObjectWarn(L, "setDepth");
        return 0;
    }
    double depth = luaL_checknumber(L, 2);
    EngineContext::core()->getSpriteManager().setDepth(sprite_lua_ptr->sprite_id, depth);
    return 0;
}

int Lua::Sprite::getPos(lua_State *L) noexcept {
    auto sprite_lua_ptr = reinterpret_cast<SpriteLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (sprite_lua_ptr->sprite_id == -1) {
        destroyedObjectWarn(L, "getPos");
        return 0;
    }
    Vector2d pos = EngineContext::core()->getSpriteManager().getPos(sprite_lua_ptr->sprite_id);
    lua_pushnumber(L, pos.x);
    lua_pushnumber(L, pos.y);
    return 2;
}

int Lua::Sprite::move(lua_State *L) noexcept {
    auto sprite_lua_ptr = reinterpret_cast<SpriteLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (sprite_lua_ptr->sprite_id == -1) {
        destroyedObjectWarn(L, "move");
        return 0;
    }
    Vector2d move = Vector2{luaL_checknumber(L, 2), luaL_checknumber(L, 3)};
    EngineContext::core()->getSpriteManager().move(sprite_lua_ptr->sprite_id, move);
    return 0;
}

int Lua::Sprite::__gc(lua_State *L) noexcept {
    Log::dbg("sprite __gc");
    destroy(L);
    return 0;
}