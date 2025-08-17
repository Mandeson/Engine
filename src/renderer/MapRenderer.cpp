#include "MapRenderer.hpp"

MapRenderer::MapRenderer(Vector2i window_size) : shader_("world") {
    a_pos_location_ = shader_.getAttribLocation("aPos");
    a_tex_coord_location_ = shader_.getAttribLocation("aTexCoord");
    u_window_size_location_ = shader_.getUniformLocation("uWindowSize");
    u_texture_size_location_ = shader_.getUniformLocation("uTextureSize");
    u_camera_pos_location_ = shader_.getUniformLocation("uCameraPos");
    u_tile_size_location_ = shader_.getUniformLocation("uTileSize");
    windowSize(window_size);
}

void MapRenderer::windowSize(Vector2i size) {
    shader_.use();
    Shader::setUniform2f(u_window_size_location_, size);
}

void MapRenderer::render(Map &map, Vector2f camera_pos) {
    for (auto &layer : map.layers_) {
        for (auto &chunk : layer.chunks) {
            for (size_t tileset_index = 0; tileset_index < chunk.buffers.size(); tileset_index++) {
                auto &buffer = chunk.buffers[tileset_index];
                if (buffer != nullptr) {
                    buffer->bind(a_pos_location_, a_tex_coord_location_, GL_STATIC_DRAW);
                    auto &tileset = map.tilesets_[tileset_index];
                    auto &texture = tileset->getTexture();
                    texture.bind();
                    shader_.use();
                    Shader::setUniform2f(u_texture_size_location_, {1.0f / texture.getSize().x, 1.0f / texture.getSize().y});
                    Shader::setUniform1f(u_tile_size_location_, tileset->getTileSize() * 2);
                    Shader::setUniform2f(u_camera_pos_location_, camera_pos);
                    buffer->render();
                }
            }
        }
    }
}
