#include "TextRenderer.hpp"
#include "../TextureBufferBuilder.hpp"

TextRenderer::TextRenderer(Vector2i window_size) : shader_("text") {
    a_pos_location_ = shader_.getAttribLocation("aPos");
    a_tex_coord_location_ = shader_.getAttribLocation("aTexCoord");
    u_window_size_location_ = shader_.getUniformLocation("uWindowSize");
    u_texture_size_location_ = shader_.getUniformLocation("uTextureSize");
    u_pos_location_ = shader_.getUniformLocation("uPos");
    u_color_location_ = shader_.getUniformLocation("uColor");
    windowSize(window_size);
}

void TextRenderer::windowSize(Vector2i size) {
    shader_.use();
    Shader::setUniform2f(u_window_size_location_, size);
}

void TextRenderer::render(Text &text, Vector2f position, Color color) {
    size_t units_count = text.render_units_.size();
    for (size_t i = 0; i < units_count; i++) {
        auto &buffer = text.render_units_[i];
        if (buffer != nullptr) {
            if (buffer->bind(a_pos_location_, a_tex_coord_location_)) { // if buffer is not empty
                glBindTexture(GL_TEXTURE_2D, text.font_atlas_.getAtlasTextureId(i));
                shader_.use();
                Shader::setUniform1f(u_texture_size_location_, 1.0f / text.font_atlas_.getTextureSize());
                Shader::setUniform2f(u_pos_location_, position);
                Shader::setUniformColor(u_color_location_, color);
                buffer->render();
            }
        }
    }
}
