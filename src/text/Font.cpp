#include "Font.hpp"
#include <format>
#include <freetype/freetype.h>
#include <freetype/fttypes.h>
#include "../util/Logger.hpp"

int Font::fonts_ = 0;
FT_Library Font::library_;

const char *Font::FreetypeInitError::what() const noexcept {
    return "Freetype initialization failed";
}

Font::FontLoadingError::FontLoadingError(std::string &&path) {
    message_ = std::format("Could not load font: {}", std::move(path));
}

const char *Font::FontLoadingError::what() const noexcept {
    return message_.c_str();
}

FontAtlas &Font::getAtlas(float font_size) {
    return atlases_.try_emplace(font_size, *this, font_size).first->second;
}

void Font::add() {
    if (fonts_ == 0) {
        if (FT_Init_FreeType(&Font::library_) != 0)
            throw FreetypeInitError();
        Log::dbg("Initialized FreeType");
    }

    fonts_++;
}

void Font::remove() {
    fonts_--;
    if (fonts_ == 0) {
        FT_Done_FreeType(Font::library_);
        Log::dbg("Terminated FreeType");
    }
}
