#include "TextureRenderer.hpp"

TextureRenderer::TextureRenderer() : shader_("texture") {
    shader_locations_ = {
        .a_pos_location_ = shader_.getAttribLocation("aPos"),
        .a_tex_coord_location_ = shader_.getAttribLocation("aTexCoord"),
        .u_window_size_location_ = shader_.getUniformLocation("uWindowSize"),
        .u_texture_size_location_ = shader_.getUniformLocation("uTextureSize"),
        .u_pos_location_ = shader_.getUniformLocation("uPos"),
        .u_scale_location_ = shader_.getUniformLocation("uScale")
    };
}

Shader &TextureRenderer::getShader() {
    return shader_;
}

TextureRenderer::ShaderLocations &TextureRenderer::getShaderLocations() {
    return shader_locations_;
}

/*void TextureRenderer::render(Sprite &sprite, Vector2f pos) {
    
}*/
