#include "WorldRenderer.hpp"
#include "MapRenderer.hpp"
#include <cmath>

WorldRenderer::WorldRenderer(PipelineState &pipeline_state, Vector2i window_size)
        : pipeline_state_(pipeline_state), map_renderer_(pipeline_state, window_size), shader_("world") {
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

void WorldRenderer::renderMap(Map &map, Vector2d camera_pos) {
    Vector2f camera_pos_screen = (static_cast<Vector2f>(camera_pos) * map.getTileSize() * pixel_scale_
            - window_size_ / 2) / pixel_scale_;
    Vector2f camera_pos_integer = Vector2{floorf(camera_pos_screen.x), floorf(camera_pos_screen.y)};
    Vector2f camera_pos_fraction = camera_pos_screen - camera_pos_integer;
    map_renderer_.renderFramebuffer(map,  camera_pos_integer);
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
    }
}

void WorldRenderer::renderSprite(Sprite &sprite, Vector2d camera_pos) {
    if (sprite.buffer_builder_.bind(a_pos_location_, a_tex_coord_location_, GL_STATIC_DRAW)) {
        sprite.tileset_.getTexture().bind(pipeline_state_);
        shader_.use(pipeline_state_);
        auto texture_size = sprite.tileset_.getTexture().getSize();
        Shader::setUniform2f(u_texture_size_location_, {
            1 / static_cast<float>(texture_size.x),
            1 / static_cast<float>(texture_size.y)
        });
        Shader::setUniform2f(u_pos_location_,
                (static_cast<Vector2f>(sprite.pos_ - camera_pos) * sprite.tileset_.getTileSize() - sprite.size_ / 2) * pixel_scale_
                + window_size_ / 2);
        Shader::setUniform1f(u_pixel_scale_location_, pixel_scale_);
        sprite.buffer_builder_.render();
    }
}

int WorldRenderer::getPixelScale() {
    return pixel_scale_;
}
