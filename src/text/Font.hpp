#pragma once

#include <exception>
#include <freetype/freetype.h>
#include <lua.hpp>
#include <string>
#include <unordered_map>
#include "FontAtlas.hpp"

class Font {
public:
    class FreetypeInitError : public std::exception {
    public:
        const char *what() const noexcept override;
    };

    class FontLoadingError : public std::exception {
    public:
        FontLoadingError(std::string &&path);
        const char *what() const noexcept override;
    private:
        std::string message_;
    };

    Font(const char *filename);
    ~Font();
    FontAtlas &getAtlas(float font_size);

    static void registerLua(lua_State *L, Font *default_font);
    static int loadLuaS(lua_State *L) noexcept;
    static int getDefaultLuaS(lua_State *L) noexcept;
    static int __gcLua(lua_State *L) noexcept;
private:
    struct FontLua {
        Font *font;
        bool destructible; //Indicates whether the object is dynamically alocated by Font::loadLuaS and requires deleting
    };

    FT_Face face_;
    std::unordered_map<float, FontAtlas> atlases_;

    static int fonts_;
    static FT_Library library_;
    static Font *default_;

    static constexpr const char *kLuaStaticMetaTable = "FontStatic";
    static constexpr const char *kLuaMetaTable = "Font";

    friend class FontAtlas;
};
