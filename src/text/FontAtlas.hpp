#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include "../OpenGL.hpp"
#include "../util/Vector.hpp"

class Font;

class FontAtlas {
public:
    struct Glyph {
        int texture_index;
        Vector2i atlas_pos;
        Vector2i size;
        Vector2i offset;
        Vector2i advance;
    };
    class GlyphNotFoundError : public std::exception {
    public:
        const char *what() const noexcept override;
    };
    class GlyphRenderError : public std::exception {
    public:
        const char *what() const noexcept override;
    };

    FontAtlas(Font &font, float font_size);
    Glyph &getGlyph(wchar_t codepoint);
    int getAtlasCount();
    GLuint getAtlasTextureId(int atlas_index);
    int getTextureSize();
private:
    struct Texture {
        Texture(int texture_size);
        ~Texture();

        GLuint texture_id_;
        bool texture_generated_ = false;
        std::vector<uint8_t> atlas_;
        bool dirty_;
    };

    Font &font_;
    float font_size_;
    int texture_size_;
    Vector2i pos_ = {1, 1};
    int row_height_ = 0;
    std::unordered_map<wchar_t, Glyph> glyphs_;
    std::vector<Texture> textures_;
};
