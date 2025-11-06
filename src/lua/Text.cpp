#include "Text.hpp"
#include "../util/Logger.hpp"
#include "../EngineContext.hpp"
#include "Font.hpp"
#include <stdexcept>

void Lua::Text::registerLua(lua_State *L) {
    luaL_newmetatable(L, kLuaStaticMetaTable);
    const struct luaL_Reg funcs[] = {
        {"new", newS},
        //{"getDefault", getDefaultS},
        {nullptr, nullptr}
    };
    luaL_setfuncs(L, funcs, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_setfield(L, -2, "Text");
}

int Lua::Text::newS(lua_State *L) noexcept {
    auto font = *reinterpret_cast<::Font **>(luaL_checkudata(L, 1, Lua::Font::kLuaMetaTable));
    if (font == nullptr) {
        lua_pop(L, 1);
        lua_pushnil(L);
        luaL_error(L, "Null font pointer Engine.Text.new()");
        return 1;
    }

    auto font_size = luaL_checknumber(L, 2);

    auto text_ptr = reinterpret_cast<TextId *>(lua_newuserdata(L, sizeof(TextId)));
    try {
        if (text_ptr == nullptr)
            throw std::runtime_error("Lua::Text::newS(): Lua userdata allocation error");
        auto core = EngineContext::core();
        *text_ptr = core->getTextManager().newObject(core->getThreadPool(), *font,static_cast<float>(font_size));
    } catch (std::exception &e) {
        lua_pop(L, 1);
        lua_pushnil(L);
        luaL_error(L, e.what());
        return 1;
    }

    Log::info("Text newS");
    if (luaL_newmetatable(L, kLuaMetaTable)) {
        const struct luaL_Reg methods[] = {
            {"setString", setString},
            {"setScale", setScale},
            {"setPos", setPos},
            {"setColor", setColor},
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

int Lua::Text::setString(lua_State *L) noexcept {
    auto text_id = *reinterpret_cast<TextId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    const char *str = luaL_checkstring(L, 2);
    EngineContext::core()->getTextManager().getObject(text_id).getText().setString(str);
    return 0;
}

int Lua::Text::setScale(lua_State *L) noexcept {
    auto text_id = *reinterpret_cast<TextId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    auto scale = luaL_checknumber(L, 2);
    EngineContext::core()->getTextManager().getObject(text_id).getText()
            .setScale(static_cast<float>(scale));
    return 0;
}

int Lua::Text::setPos(lua_State *L) noexcept {
    auto text_id = *reinterpret_cast<TextId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    Vector2d pos = Vector2{luaL_checknumber(L, 2), luaL_checknumber(L, 3)};
    EngineContext::core()->getTextManager().getObject(text_id).setPos(pos);
    return 0;
}

int Lua::Text::setColor(lua_State *L) noexcept {
    auto text_id = *reinterpret_cast<TextId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    Color color = {
        static_cast<uint8_t>(luaL_checkinteger(L, 2)),
        static_cast<uint8_t>(luaL_checkinteger(L, 3)),
        static_cast<uint8_t>(luaL_checkinteger(L, 4))};
    EngineContext::core()->getTextManager().getObject(text_id).setColor(color);
    return 0;
}

int Lua::Text::__gc(lua_State *L) noexcept {
    Log::info("Text __gc");
    auto *text_id = reinterpret_cast<TextId *>(luaL_checkudata(L, 1, kLuaMetaTable));
    EngineContext::core()->getTextManager().destroyObject(*text_id);
    return 0;
}