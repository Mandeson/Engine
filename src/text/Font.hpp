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

    Font(const Font &) = delete;
    Font& operator=(const Font&) = delete;

    ~Font();
    FontAtlas &getAtlas(float font_size);
private:
        FT_Face face_;
    std::unordered_map<float, FontAtlas> atlases_;

    static int fonts_;
    static FT_Library library_;

    friend class FontAtlas;
};
