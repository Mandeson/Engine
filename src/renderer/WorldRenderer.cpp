#include "WorldRenderer.hpp"
#include "MapRenderer.hpp"
#include <cmath>

WorldRenderer::WorldRenderer(Vector2i window_size) : map_renderer_(window_size), shader_("world") {
    a_pos_location_ = shader_.getAttribLocation("aPos");
    a_tex_coord_location_ = shader_.getAttribLocation("aTexCoord");
    u_window_size_location_ = shader_.getUniformLocation("uWindowSize");
    u_texture_size_location_ = shader_.getUniformLocation("uTextureSize");
    u_pos_location_ = shader_.getUniformLocation("uPos");
    u_pixel_scale_location_ = shader_.getUniformLocation("uPixelScale");
    setWindowSize(window_size);
}

void WorldRenderer::windowSize(Vector2i size) {
    map_renderer_.windowSize(size);
    setWindowSize(size);
}

void WorldRenderer::render(Map &map, Vector2f camera_pos) {
    Vector2f camera_pos_screen = camera_pos * static_cast<float>(map.getTileSize());
    Vector2f camera_pos_integer = Vector2{floorf(camera_pos_screen.x), floorf(camera_pos_screen.y)};
    map_renderer_.renderFramebuffer(map,  camera_pos_integer);
    Vector2f camera_pos_fraction = camera_pos_screen - camera_pos_integer;
    auto &screen_buffer = map_renderer_.screen_buffer_;
    if (screen_buffer.bind(a_pos_location_, a_tex_coord_location_, GL_STATIC_DRAW)) {
        map_renderer_.getFramebufferTexture().bind();
        shader_.use();
        Shader::setUniform2f(u_texture_size_location_, {
            1 / static_cast<float>(map_renderer_.framebuffer_size_.x),
            1 / static_cast<float>(map_renderer_.framebuffer_size_.y)
        });
        Shader::setUniform2f(u_pos_location_, camera_pos_fraction * -map_renderer_.pixel_scale_);
        Shader::setUniform1f(u_pixel_scale_location_, map_renderer_.pixel_scale_);
        screen_buffer.render();
    }
}

void WorldRenderer::setWindowSize(Vector2i size) {
    shader_.use();
    Shader::setUniform2f(u_window_size_location_, size);
}
