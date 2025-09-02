#include "FontAtlas.hpp"
#include "../util/Logger.hpp"
#include <cstdint>
#include <freetype/fttypes.h>
#include <mutex>
#include <vector>
#include "Font.hpp"

std::mutex FontAtlas::freetype_mutex_;

const char *FontAtlas::GlyphNotFoundError::what() const noexcept {
    return "Could not load a font character";
}

const char *FontAtlas::GlyphRenderError::what() const noexcept {
    return "Could not render a font character";
}

FontAtlas::FontAtlas(Font &font, float font_size) : font_(font), font_size_(font_size), texture_size_(256) {
    if (font_size >= 50.0f)
		texture_size_ *= 2;
	if (font_size >= 100.0f)
		texture_size_ *= 2;
	if (font_size >= 200.0f)
		texture_size_ *= 2;
    Log::dbg("Creatng new FontAtlas");
    textures_.emplace_back(texture_size_);
}

FontAtlas::Glyph &FontAtlas::getGlyph(wchar_t codepoint) {
    std::scoped_lock lock{mutex_};
    auto it = glyphs_.find(codepoint);
    if (it == glyphs_.end()) { // Glyph not yet rendered
        std::scoped_lock freetype_lock{freetype_mutex_};
        FT_Set_Char_Size(font_.face_, 0, static_cast<int>(font_size_ * 64), 0, 0);
		Log::dbg( "Rendering glyph: {}", static_cast<uint32_t>(codepoint));
		auto glyph_index = FT_Get_Char_Index(font_.face_, codepoint);
		if (FT_Load_Glyph(font_.face_, glyph_index, FT_LOAD_DEFAULT) != 0)
			throw GlyphNotFoundError();
		if (FT_Render_Glyph(font_.face_->glyph, FT_RENDER_MODE_NORMAL) != 0)
			throw GlyphRenderError();
		FT_GlyphSlot g = font_.face_->glyph;
        Vector2i glyph_size = {static_cast<int>(g->bitmap.width), static_cast<int>(g->bitmap.rows)};
		if (pos_.x + glyph_size.x >= texture_size_) { // No space left in the current row
			pos_.x = kMargin;
			pos_.y += row_height_ + kMargin;
			row_height_ = 0;
		}
		if (pos_.y + glyph_size.y >= texture_size_) { // Texture full
            // TODO: if it's the first glyph in this texture, throw exception
			Log::dbg("Font atlas full");
            pos_ = {kMargin, kMargin};
            row_height_ = 0;
            textures_.emplace_back(texture_size_);
		}
        auto &last_texture = textures_.back();
        for (int y = 0; y < glyph_size.y; y++) {
			for (int x = 0; x < glyph_size.x; x++) {
				last_texture.atlas_[(pos_.y + y) * texture_size_ + (pos_.x + x)] = g->bitmap.buffer[y * g->bitmap.width + x];
			}
		}
		row_height_ = std::max(row_height_, static_cast<int>(g->bitmap.rows));
        auto &glyph = glyphs_.emplace(codepoint, Glyph{
                .texture_index = static_cast<int>(textures_.size() - 1),
                .atlas_pos = pos_,
                .size = Vector2{g->bitmap.width, g->bitmap.rows},
                .offset = Vector2{g->bitmap_left, g->bitmap_top},
                .advance = Vector2{g->advance.x, g->advance.y}}).first->second;
		pos_.x += g->bitmap.width + kMargin;
        last_texture.dirty_ = true;
        return glyph;
    } else {
        return it->second;
    }
}

GLuint FontAtlas::getAtlasTextureId(int texture_index) {
    std::scoped_lock lock{mutex_};
    for (int i = 0; i < static_cast<int>(textures_.size()) - 1; i++) { // Not the last, actively changed atlas
        auto &texture = textures_[i];
        if (texture.atlas_.size() && texture.texture_generated_ && !texture.dirty_)
            texture.atlas_ = std::vector<uint8_t>(); // Deallocate memory
    }

    auto &texture = textures_[texture_index];
    if (!texture.texture_generated_) {
        glGenTextures(1, &texture.texture_id_);
        glBindTexture(GL_TEXTURE_2D, texture.texture_id_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, texture_size_,
                texture_size_, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                reinterpret_cast<void *>(&texture.atlas_[0]));
        texture.texture_generated_ = true;
    } else {
        if (texture.dirty_) {
            glBindTexture(GL_TEXTURE_2D, texture.texture_id_);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture_size_,
                    texture_size_, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                    reinterpret_cast<void *>(&texture.atlas_[0]));
        }
    }
    texture.dirty_ = false;
    return texture.texture_id_;
}

int FontAtlas::getTextureSize() {
    return  texture_size_;
}

FontAtlas::Texture::Texture(int texture_size) : atlas_(texture_size * texture_size) { }

FontAtlas::Texture::~Texture() {
    if (texture_generated_)
        glDeleteTextures(1, &texture_id_);
}
