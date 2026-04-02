#include "SpriteRenderer.hpp"

void SpriteRenderer::renderSprites(TextureRenderer &texture_renderer, PipelineState &pipeline_state,
        Vector2i window_size, SpriteManager &manager) {
    size_t prev_quad_count = 0;
    manager.forEachSpriteDepthSorted([&prev_quad_count, this] (Sprite &sprite) {
        if (!sprite.ready())
            return;
        auto *new_texture = &sprite.getTexture();
        if (temp_render_units_.size() == 0) {
            temp_render_units_.push_back(SpriteRenderUnit{.texture = *new_texture, .quad_offset = 0, .quad_count = 0});
        } else if (new_texture != &temp_render_units_.back().texture) {
            auto quad_count = temp_sprite_buffer_.getQuadCount();
            temp_render_units_.back().quad_count = quad_count - prev_quad_count;
            prev_quad_count = quad_count;
            temp_render_units_.push_back(SpriteRenderUnit{.texture = *new_texture, .quad_offset = quad_count, .quad_count = 0});
        }

        temp_sprite_buffer_.addRectangle(sprite.getPos(), sprite.getSize(), sprite.getTextureRect());
    });
    if (temp_render_units_.size() == 0)
        return;
    temp_render_units_.back().quad_count = temp_sprite_buffer_.getQuadCount() - prev_quad_count;
    temp_sprite_buffer_.end();
    for (const auto &unit : temp_render_units_)
        renderSpriteUnit(texture_renderer, pipeline_state, window_size, unit);
    temp_render_units_.clear();
    temp_sprite_buffer_.clear();
}

void SpriteRenderer::renderSpriteUnit(TextureRenderer &texture_renderer, PipelineState &pipeline_state,
        Vector2i window_size, const SpriteRenderUnit &unit) {
    auto &locations = texture_renderer.getShaderLocations();
    if (temp_sprite_buffer_.bind(locations.a_pos_location_, locations.a_tex_coord_location_, GL_STREAM_DRAW)) {
        unit.texture.bind(pipeline_state);
        texture_renderer.getShader().use(pipeline_state);
        Vector2f texture_size = unit.texture.getSize();
        Shader::setUniform2f(locations.u_window_size_location_, window_size);
        Shader::setUniform2f(locations.u_texture_size_location_, Vector2{1.0f / texture_size.x, 1.0f / texture_size.y});
        Shader::setUniform2f(locations.u_pos_location_, Vector2{0, 0});
        Shader::setUniform2f(locations.u_scale_location_, Vector2f{1.0f, 1.0f});
        temp_sprite_buffer_.renderRange(unit.quad_offset, unit.quad_count);
        temp_sprite_buffer_.unbind();
    }
}
