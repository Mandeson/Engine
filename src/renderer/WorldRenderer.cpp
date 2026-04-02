#include "WorldRenderer.hpp"
#include "MapRenderer.hpp"
#include "TextureRenderer.hpp"
#include <cmath>

WorldRenderer::WorldRenderer(PipelineState &pipeline_state, Vector2i window_size)
        : pipeline_state_(pipeline_state), map_renderer_(pipeline_state), shader_("world") {
    a_pos_location_ = shader_.getAttribLocation("aPos");
    a_tex_coord_location_ = shader_.getAttribLocation("aTexCoord");
    u_window_size_location_ = shader_.getUniformLocation("uWindowSize");
    u_texture_size_location_ = shader_.getUniformLocation("uTextureSize");
    u_pos_location_ = shader_.getUniformLocation("uPos");
    u_pixel_scale_location_ = shader_.getUniformLocation("uPixelScale");
    windowSize(window_size);
}

void WorldRenderer::windowSize(Vector2i size) {
    window_size_ = size;
    float pixel_scale_float = std::round(std::min(size.x, size.y) / static_cast<float>(kPixelScaleDivider));
    pixel_scale_ = std::max(1, static_cast<int>(pixel_scale_float));
    shader_.use(pipeline_state_);
    Shader::setUniform2f(u_window_size_location_, size);
    map_renderer_.build(size, pixel_scale_);
}

void WorldRenderer::renderMap(TextureRenderer &texture_renderer, Map &map, Vector2d camera_pos) {
    Vector2f camera_pos_screen = (static_cast<Vector2f>(camera_pos) * map.getTileSize() * pixel_scale_
            - window_size_ / 2) / pixel_scale_;
    Vector2f camera_pos_integer = Vector2{floorf(camera_pos_screen.x), floorf(camera_pos_screen.y)};
    Vector2f camera_pos_fraction = camera_pos_screen - camera_pos_integer;
    map_renderer_.renderFramebuffer(texture_renderer, map,  camera_pos_integer);
    auto &screen_buffer = map_renderer_.screen_buffer_;
    if (screen_buffer.bind(a_pos_location_, a_tex_coord_location_, GL_STATIC_DRAW)) {
        map_renderer_.getFramebufferTexture().bind(pipeline_state_);
        shader_.use(pipeline_state_);
        Shader::setUniform2f(u_texture_size_location_, {
            1 / static_cast<float>(map_renderer_.framebuffer_size_.x),
            1 / static_cast<float>(map_renderer_.framebuffer_size_.y)
        });
        auto shift = camera_pos_fraction * -pixel_scale_;
        //shift = Vector2{roundf(shift.x), roundf(shift.y)};
        Shader::setUniform2f(u_pos_location_, shift);
        Shader::setUniform1f(u_pixel_scale_location_, pixel_scale_);
        screen_buffer.render();
        screen_buffer.unbind();
    }
}

void WorldRenderer::renderTilesetSprites(TilesetSpriteManager &manager, Vector2d camera_pos) {
    size_t prev_quad_count = 0;
    manager.forEachSpriteDepthSorted([&prev_quad_count, &camera_pos, this] (TilesetSprite &sprite) {
        if (!sprite.ready())
            return;
        auto *new_texture = &sprite.getTileset().getTexture();
        if (temp_render_units_.size() == 0) {
            temp_render_units_.push_back(SpriteRenderUnit{.texture = *new_texture, .quad_offset = 0, .quad_count = 0});
        } else if (new_texture != &temp_render_units_.back().texture) {
            auto quad_count = temp_sprite_buffer_.getQuadCount();
            temp_render_units_.back().quad_count = quad_count - prev_quad_count;
            prev_quad_count = quad_count;
            temp_render_units_.push_back(SpriteRenderUnit{.texture = *new_texture, .quad_offset = quad_count, .quad_count = 0});
        }

        auto tile_size = sprite.getTileset().getTileSize();
        auto pos = (static_cast<Vector2f>(sprite.getPos() - camera_pos) * tile_size) * pixel_scale_ + window_size_ / 2;
        // pos = pos * tile_size;
        // pos = Vector2{round(pos.x), round(pos.y)};
        // pos = pos / tile_size;
        temp_sprite_buffer_.addRectangle(pos / pixel_scale_, sprite.getSize(), sprite.getTextureRect());
    });
    if (temp_render_units_.size() == 0)
        return;
    temp_render_units_.back().quad_count = temp_sprite_buffer_.getQuadCount() - prev_quad_count;
    temp_sprite_buffer_.end();
    for (const auto &unit : temp_render_units_)
        renderSpriteUnit(unit);
    temp_render_units_.clear();
    temp_sprite_buffer_.clear();
}

int WorldRenderer::getPixelScale() {
    return pixel_scale_;
}

void WorldRenderer::renderSpriteUnit(const SpriteRenderUnit &unit) {
    if (temp_sprite_buffer_.bind(a_pos_location_, a_tex_coord_location_, GL_STREAM_DRAW)) {
        unit.texture.bind(pipeline_state_);
        shader_.use(pipeline_state_);
        auto texture_size = unit.texture.getSize();
        Shader::setUniform2f(u_texture_size_location_, {
            1 / static_cast<float>(texture_size.x),
            1 / static_cast<float>(texture_size.y)
        });
        Shader::setUniform2f(u_pos_location_, Vector2{0, 0});
        Shader::setUniform1f(u_pixel_scale_location_, pixel_scale_);
        temp_sprite_buffer_.renderRange(unit.quad_offset, unit.quad_count);
        temp_sprite_buffer_.unbind();
    }
}
