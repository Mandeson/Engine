#include "SpriteRenderer.hpp"

void SpriteRenderer::render(TextureRenderer &texture_renderer, PipelineState &pipeline_state, Vector2i window_size, Sprite &sprite) {
    auto &locations = texture_renderer.getShaderLocations();
    auto &buffer = sprite.getBuffer();
    if (buffer.bind(locations.a_pos_location_, locations.a_tex_coord_location_)) {
        auto &texture = sprite.getTexture();
        texture.bind(pipeline_state);
        texture_renderer.getShader().use(pipeline_state);
        Vector2f texture_size = texture.getSize();
        Shader::setUniform2f(locations.u_window_size_location_, window_size);
        Shader::setUniform2f(locations.u_texture_size_location_, Vector2{1.0f / texture_size.x, 1.0f / texture_size.y});
        Shader::setUniform2f(locations.u_pos_location_, sprite.getPos());
        Shader::setUniform2f(locations.u_scale_location_, sprite.getSize());
        buffer.render();
    }
}
