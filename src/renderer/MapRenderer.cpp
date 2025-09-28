#include "MapRenderer.hpp"
#include <cmath>
#include <stdexcept>

void MapRenderer::FramebufferTexture::bind() {
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture_id);
}

MapRenderer::MapRenderer(Vector2i window_size) : shader_("map") {
    a_pos_location_ = shader_.getAttribLocation("aPos");
    a_tex_coord_location_ = shader_.getAttribLocation("aTexCoord");
    u_window_size_location_ = shader_.getUniformLocation("uWindowSize");
    u_texture_size_location_ = shader_.getUniformLocation("uTextureSize");
    u_camera_pos_location_ = shader_.getUniformLocation("uCameraPos");
    u_tile_size_location_ = shader_.getUniformLocation("uTileSize");
    OpenGL::glGenFramebuffersPtr(1, &FBO_);
    OpenGL::glBindFramebufferPtr(GL_FRAMEBUFFER, FBO_);
    glGenTextures(1, &framebuffer_texture_id_);
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture_id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

MapRenderer::~MapRenderer() {
    glDeleteTextures(1, &framebuffer_texture_id_);
    OpenGL::glDeleteFramebuffersPtr(1, &FBO_);
}

void MapRenderer::build(Vector2i size, int pixel_scale) {
    window_size_ = size;
    float pixel_scale_f = static_cast<float>(pixel_scale);
    framebuffer_size_ = Vector2{static_cast<int>(ceilf(size.x / pixel_scale_f + 1.0f)),
            static_cast<int>(ceilf(size.y / pixel_scale_f + 1.0f))};

    //Log::info("{} {}", framebuffer_size_.x, framebuffer_size_.y);

    glBindTexture(GL_TEXTURE_2D, framebuffer_texture_id_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, framebuffer_size_.x, framebuffer_size_.y,
            0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    OpenGL::glBindFramebufferPtr(GL_FRAMEBUFFER, FBO_);
    OpenGL::glFramebufferTexture2DPtr(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_texture_id_, 0);
    if (OpenGL::glCheckFramebufferStatusPtr(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glDeleteTextures(1, &framebuffer_texture_id_);
        OpenGL::glDeleteFramebuffersPtr(1, &FBO_);
        throw std::runtime_error("Could not initialize an OpenGL Framebuffer");
    }
    OpenGL::glBindFramebufferPtr(GL_FRAMEBUFFER, 0);
    screen_buffer_.clear();
    screen_buffer_.addRectangle({0, 0}, framebuffer_size_, TextureRect{{0, 0},framebuffer_size_ }, false, true);
    screen_buffer_.end();

    shader_.use();
    Shader::setUniform2f(u_window_size_location_, framebuffer_size_);
}

void MapRenderer::renderFramebuffer(Map &map, Vector2f camera_pos) {
    OpenGL::glBindFramebufferPtr(GL_FRAMEBUFFER, FBO_);
    glViewport(0, 0, framebuffer_size_.x, framebuffer_size_.y);
    glClear(GL_COLOR_BUFFER_BIT);
    shader_.use();
    Shader::setUniform1f(u_tile_size_location_, map.getTileSize());
    Shader::setUniform2f(u_camera_pos_location_, camera_pos);
    for (auto &layer : map.layers_) {
        for (auto &chunk : layer.chunks) {
            for (size_t tileset_index = 0; tileset_index < chunk.buffers.size(); tileset_index++) {
                auto &buffer = chunk.buffers[tileset_index];
                if (buffer != nullptr) {
                    buffer->bind(a_pos_location_, a_tex_coord_location_, GL_STATIC_DRAW);
                    auto &tileset = map.tilesets_[tileset_index];
                    auto &texture = tileset->getTexture();
                    texture.bind();
                    Shader::setUniform2f(u_texture_size_location_, {1.0f / texture.getSize().x, 1.0f / texture.getSize().y});
                    buffer->render();
                }
            }
        }
    }
    OpenGL::glBindFramebufferPtr(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window_size_.x, window_size_.y);
}

MapRenderer::FramebufferTexture MapRenderer::getFramebufferTexture() {
    return MapRenderer::FramebufferTexture{framebuffer_texture_id_};
}
