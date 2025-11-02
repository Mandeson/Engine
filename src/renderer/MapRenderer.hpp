#pragma once

#include "../PipelineState.hpp"
#include "TextureRenderer.hpp"
#include "../world/Map.hpp"

class WorldRenderer;

class MapRenderer {
public:
    struct FramebufferTexture {
        GLuint framebuffer_texture_id;
        void bind(PipelineState &pipeline_state);
    };
    MapRenderer(PipelineState &pipeline_state);
    ~MapRenderer();
    void build(Vector2i size, int pixel_scale);
    void renderFramebuffer(TextureRenderer &texture_renderer, Map &map, Vector2f camera_pos);
    FramebufferTexture getFramebufferTexture();
private:
    PipelineState &pipeline_state_;
    Vector2i window_size_;
    Vector2i framebuffer_size_;
    GLuint FBO_;
    GLuint framebuffer_texture_id_;
    TextureBufferBuilder screen_buffer_;

    friend class WorldRenderer;
};
