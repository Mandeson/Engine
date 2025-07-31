#include "TextRenderer.hpp"
#include "../TextureBufferBuilder.hpp"

TextRenderer::TextRenderer() : shader_("text") {
    a_pos_location_ = shader_.getAttribLocation("aPos");
    a_tex_coord_location_ = shader_.getAttribLocation("aTexCoord");
    u_window_size_location_ = shader_.getUniformLocation("uWindowSize");
    u_texture_size_location_ = shader_.getUniformLocation("uTextureSize");
    u_pos_location_ = shader_.getUniformLocation("uPos");
    u_color_location_ = shader_.getUniformLocation("uColor");
}

void TextRenderer::render(Text &text) {
    size_t units_count = text.render_units_.size();
    for (size_t i = 0; i < units_count; i++) {
        auto &buffer = text.render_units_[i];
        if (buffer->bind(a_pos_location_, a_tex_coord_location_)) { // if buffer is not empty
            glBindTexture(GL_TEXTURE_2D, text.font_atlas_.getAtlasTextureId(i));
            shader_.use();
            shader_.setUniform2f(u_window_size_location_, {800, 600});
            shader_.setUniform1f(u_texture_size_location_, 1.0f / text.font_atlas_.getTextureSize());
            shader_.setUniform2f(u_pos_location_, {0, 0});
            shader_.setUniformColor(u_color_location_, {255, 255, 255});
            buffer->render();
        }
    }
}
