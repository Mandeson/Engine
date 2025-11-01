#include "MapRenderer.hpp"
#include <cmath>
#include <stdexcept>

void MapRenderer::FramebufferTexture::bind(PipelineState &pipeline_state) {
    pipeline_state.bindTexture(framebuffer_texture_id);
}

MapRenderer::MapRenderer(PipelineState &pipeline_state, Vector2i window_size) : pipeline_state_(pipeline_state) {
    OpenGL::glGenFramebuffersPtr(1, &FBO_);
    OpenGL::glBindFramebufferPtr(GL_FRAMEBUFFER, FBO_);
    glGenTextures(1, &framebuffer_texture_id_);
    pipeline_state_.bindTexture(framebuffer_texture_id_);
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

    pipeline_state_.bindTexture(framebuffer_texture_id_);
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
}

void MapRenderer::renderFramebuffer(TextureRenderer &texture_renderer, Map &map, Vector2f camera_pos) {
    OpenGL::glBindFramebufferPtr(GL_FRAMEBUFFER, FBO_);
    glViewport(0, 0, framebuffer_size_.x, framebuffer_size_.y);
    glClear(GL_COLOR_BUFFER_BIT);
    texture_renderer.getShader().use(pipeline_state_);
    auto &locations = texture_renderer.getShaderLocations();
    Shader::setUniform2f(locations.u_window_size_location_, framebuffer_size_);
    float tile_size = static_cast<float>(map.getTileSize());
    Shader::setUniform2f(locations.u_scale_location_, Vector2{tile_size, tile_size});
    Shader::setUniform2f(locations.u_pos_location_, -camera_pos);
    for (auto &layer : map.layers_) {
        for (auto &chunk : layer.chunks) {
            for (size_t tileset_index = 0; tileset_index < chunk.buffers.size(); tileset_index++) {
                auto &buffer = chunk.buffers[tileset_index];
                if (buffer != nullptr) {
                    buffer->bind(locations.a_pos_location_, locations.a_tex_coord_location_, GL_STATIC_DRAW);
                    auto &tileset = map.tilesets_[tileset_index];
                    auto &texture = tileset->getTexture();
                    texture.bind(pipeline_state_);
                    Shader::setUniform2f(locations.u_texture_size_location_, {1.0f / texture.getSize().x, 1.0f / texture.getSize().y});
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
