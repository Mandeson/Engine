#include "Font.hpp"
#include "../text/Font.hpp"
#include "../util/Logger.hpp"
#include "../EngineContext.hpp"

struct FontLua {
    ::Font *font;
    bool destructible; //Indicates whether the object is dynamically alocated by Font::loadLuaS and requires deleting
};

void Lua::Font::registerLua(lua_State *L) {
    lua_getglobal(L, "Engine");
    luaL_newmetatable(L, kLuaStaticMetaTable);
    const struct luaL_Reg funcs[] = {
        {"load", loadS},
        {"getDefault", getDefaultS},
        {nullptr, nullptr}
    };
    luaL_setfuncs(L, funcs, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_setfield(L, -2, "Font");
    lua_pop(L, 1);
}

int Lua::Font::loadS(lua_State *L) noexcept {
    const char *name = luaL_checkstring(L, 1);
    FontLua *ptr = reinterpret_cast<FontLua *>(lua_newuserdata(L, sizeof(FontLua)));
    try {
        ptr->font = new ::Font(name);
    } catch (std::exception &e) {
        lua_pop(L, 1);
        lua_pushnil(L);
        luaL_error(L, e.what());
        return 1;
    }
    ptr->destructible = true;
    Log::dbg("font loadLua");
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

int Lua::Font::getDefaultS(lua_State *L) noexcept {
    FontLua *ptr = reinterpret_cast<FontLua *>(lua_newuserdata(L, sizeof(FontLua)));
    ptr->font = &EngineContext::game()->getDefaultFont();
    ptr->destructible = false;
    Log::dbg("font getDefaultLua");
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

int Lua::Font::__gc(lua_State *L) noexcept {
    auto *ptr = reinterpret_cast<FontLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (ptr->destructible)
        delete ptr->font;
    ptr->font = nullptr;
    return 0;
}