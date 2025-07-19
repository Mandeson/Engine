#include "Font.hpp"
#include <exception>
#include <format>
#include <lua.h>
#include "../util/Logger.hpp"

int Font::fonts_ = 0;
FT_Library Font::library_;
Font *Font::default_ = nullptr;

const char *Font::FreetypeInitError::what() const noexcept {
    return "Freetype initialization failed";
}

Font::FontLoadingError::FontLoadingError(std::string &&path) {
    message_ = std::format("Could not load font: {}", std::move(path));
}

const char *Font::FontLoadingError::what() const noexcept {
    return message_.c_str();
}

Font::Font(const char *filename) {
    if (fonts_ == 0) {
        if (FT_Init_FreeType(&Font::library_) != 0)
            throw FreetypeInitError();
        Log::dbg("Initialized FreeType");
    }
    fonts_++;

    std::string path = std::string("assets/fonts/") + filename;
    if (FT_New_Face(library_, path.c_str(), 0, &face_) != 0)
		throw FontLoadingError(std::move(path));
    Log::dbg("Created font face");
}

Font::~Font() {
    FT_Done_Face(face_);
    Log::dbg("Destroyed font face");

    fonts_--;
    if (fonts_ == 0) {
        Log::dbg("Terminated FreeType");
    }
}

FontAtlas &Font::getAtlas(float font_size) {
    return atlases_.try_emplace(font_size, *this, font_size).first->second;
}

void Font::registerLua(lua_State *L, Font *default_font) {
    default_ = default_font;
    lua_getglobal(L, "Engine");
    luaL_newmetatable(L, kLuaStaticMetaTable);
    const struct luaL_Reg funcs[] = {
        {"load", Font::loadLuaS},
        {"getDefault", Font::getDefaultLuaS},
        {nullptr, nullptr}
    };
    luaL_setfuncs(L, funcs, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_setfield(L, -2, "Font");
    lua_pop(L, 1);
}

int Font::loadLuaS(lua_State *L) noexcept {
    const char *name = luaL_checkstring(L, 1);
    FontLua *ptr = reinterpret_cast<FontLua *>(lua_newuserdata(L, sizeof(FontLua)));
    try {
        ptr->font = new Font(name);
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
            {"__gc", Font::__gcLua},
            {nullptr, nullptr}
        };
        luaL_setfuncs(L, methods, 0);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
    }
    lua_setmetatable(L, -2);
    return 1;
}

int Font::getDefaultLuaS(lua_State *L) noexcept {
    FontLua *ptr = reinterpret_cast<FontLua *>(lua_newuserdata(L, sizeof(FontLua)));
    ptr->font = default_;
    ptr->destructible = false;
    Log::dbg("font getDefaultLua");
    if (luaL_newmetatable(L, kLuaMetaTable)) {
        const struct luaL_Reg methods[] = {
            {"__gc", Font::__gcLua},
            {nullptr, nullptr}
        };
        luaL_setfuncs(L, methods, 0);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
    }
    lua_setmetatable(L, -2);
    return 1;
}

int Font::__gcLua(lua_State *L) noexcept {
    auto *ptr = reinterpret_cast<FontLua *>(luaL_checkudata(L, 1, kLuaMetaTable));
    if (ptr->destructible)
        delete ptr->font;
    ptr->font = nullptr;
    return 0;
}
