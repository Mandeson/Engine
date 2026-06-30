#include "Text.hpp"
#include "../util/Logger.hpp"
#include "../EngineContext.hpp"
#include "Common.hpp"
#include "Font.hpp"

struct TextLua {
    TextId text_id = -1;
    int font_lua_ref = LUA_NOREF; // Lua reference to the associated Font object
};

static void destroyedObjectError(lua_State *L, std::string_view method_name) {
    auto str = Lua::Common::methodCallOnDestroyedObjectErrorText("Text", method_name);
    luaL_error(L, str.c_str());
}

void Lua::Text::registerLua(lua_State *L) {
    luaL_newmetatable(L, kLuaStaticMetaTable);
    const struct luaL_Reg funcs[] = {
        {"new", newS},
        {nullptr, nullptr}
    };
    luaL_setfuncs(L, funcs, 0);
    lua_newtable(L);
    lua_pushinteger(L, static_cast<lua_Integer>(::Text::Alignment::kLeft));
    lua_setfield(L, -2, "Left");
    lua_pushinteger(L, static_cast<lua_Integer>(::Text::Alignment::kCenter));
    lua_setfield(L, -2, "Center");
    lua_setfield(L, -2, "Alignment");
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

    auto text_lua_ptr = reinterpret_cast<TextLua *>(lua_newuserdata(L, sizeof(TextLua)));
    *text_lua_ptr = TextLua{}; // Initialize fields

    if (lua_gettop(L) >= 1 + 3) { // third argument: str
        std::string str{luaL_checkstring(L, 3)};

        auto alignment = ::Text::Alignment::kLeft;
        if (lua_gettop(L) >= 1 + 4) { // fourth argument: text alignment
            alignment = static_cast<::Text::Alignment>(luaL_checknumber(L, 4));
        }
        auto max_width = INT_MAX;
        if (lua_gettop(L) >= 1 + 5) { // fifth argument: max width
            max_width = static_cast<int>(luaL_checknumber(L, 5));
        }

        try {
            auto core = EngineContext::core();
            text_lua_ptr->text_id = core->getTextManager().newObject(core->getThreadPool(),
                    *font,static_cast<float>(font_size), str, alignment, max_width);
        } catch (std::exception &e) {
            lua_pop(L, 1);
            lua_pushnil(L);
            luaL_error(L, e.what());
            return 1;
        }
    } else {
        try {
            auto core = EngineContext::core();
            text_lua_ptr->text_id = core->getTextManager().newObject(core->getThreadPool(), *font,static_cast<float>(font_size));
        } catch (std::exception &e) {
            lua_pop(L, 1);
            lua_pushnil(L);
            luaL_error(L, e.what());
            return 1;
        }
    }

    lua_pushvalue(L, 1);
    text_lua_ptr->font_lua_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    Log::dbg("text newS");
    if (luaL_newmetatable(L, kLuaMetaTable)) {
        const struct luaL_Reg methods[] = {
            {"ready", ready},
            {"destroy", destroy},
            {"setVisible", setVisible},
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

int Lua::Text::ready(lua_State *L) noexcept {
    auto text_lua_ptr = reinterpret_cast<TextLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (text_lua_ptr->text_id == -1) {
        destroyedObjectError(L, "ready");
        lua_pushnil(L);
        return 1;
    }
    bool ready = EngineContext::core()->getTextManager().get(text_lua_ptr->text_id).getText().ready();
    lua_pushinteger(L, static_cast<int>(ready));
    return 1;
}

int Lua::Text::destroy(lua_State *L) noexcept {
    auto *text_lua_ptr = reinterpret_cast<TextLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (text_lua_ptr->text_id != -1) {
        EngineContext::core()->getTextManager().destroyObject(text_lua_ptr->text_id);
        text_lua_ptr->text_id = -1;
        if (text_lua_ptr->font_lua_ref != LUA_NOREF) {
            luaL_unref(L, LUA_REGISTRYINDEX, text_lua_ptr->font_lua_ref);
            text_lua_ptr->font_lua_ref = LUA_NOREF;
        }
    }
    return 0;
}

int Lua::Text::setVisible(lua_State *L) noexcept {
    auto text_lua_ptr = reinterpret_cast<TextLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (text_lua_ptr->text_id == -1) {
        destroyedObjectError(L, "setVisible");
        return 0;
    }
    bool visible = static_cast<bool>(luaL_checkinteger(L, 2));
    EngineContext::core()->getTextManager().setVisible(text_lua_ptr->text_id, visible);
    return 0;
}

int Lua::Text::setString(lua_State *L) noexcept {
    auto text_lua_ptr = reinterpret_cast<TextLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (text_lua_ptr->text_id == -1) {
        destroyedObjectError(L, "setString");
        return 0;
    }
    const char *str = luaL_checkstring(L, 2);
    EngineContext::core()->getTextManager().get(text_lua_ptr->text_id).getText().setString(str);
    return 0;
}

int Lua::Text::setScale(lua_State *L) noexcept {
    auto text_lua_ptr = reinterpret_cast<TextLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (text_lua_ptr->text_id == -1) {
        destroyedObjectError(L, "setScale");
        return 0;
    }
    auto scale = luaL_checknumber(L, 2);
    EngineContext::core()->getTextManager().get(text_lua_ptr->text_id).getText()
            .setScale(static_cast<float>(scale));
    return 0;
}

int Lua::Text::setPos(lua_State *L) noexcept {
    auto text_lua_ptr = reinterpret_cast<TextLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (text_lua_ptr->text_id == -1) {
        destroyedObjectError(L, "setPos");
        return 0;
    }
    Vector2d pos = Vector2{luaL_checknumber(L, 2), luaL_checknumber(L, 3)};
    EngineContext::core()->getTextManager().get(text_lua_ptr->text_id).setPos(pos);
    return 0;
}

int Lua::Text::setColor(lua_State *L) noexcept {
    auto text_lua_ptr = reinterpret_cast<TextLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (text_lua_ptr->text_id == -1) {
        destroyedObjectError(L, "setColor");
        return 0;
    }
    Color color = {
        static_cast<uint8_t>(luaL_checkinteger(L, 2)),
        static_cast<uint8_t>(luaL_checkinteger(L, 3)),
        static_cast<uint8_t>(luaL_checkinteger(L, 4))};
    EngineContext::core()->getTextManager().get(text_lua_ptr->text_id).setColor(color);
    return 0;
}

int Lua::Text::__gc(lua_State *L) noexcept {
    Log::dbg("text __gc");
    destroy(L);
    return 0;
}