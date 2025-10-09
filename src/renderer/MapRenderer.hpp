#pragma once

#include "../PipelineState.hpp"
#include "../Shader.hpp"
#include "../world/Map.hpp"

class WorldRenderer;

class MapRenderer {
public:
    struct FramebufferTexture {
        GLuint framebuffer_texture_id;
        void bind(PipelineState &pipeline_state);
    };
    MapRenderer(PipelineState &pipeline_state, Vector2i window_size);
    ~MapRenderer();
    void build(Vector2i size, int pixel_scale);
    void renderFramebuffer(Map &map, Vector2f camera_pos);
    FramebufferTexture getFramebufferTexture();
private:
    PipelineState &pipeline_state_;
    Shader shader_;
    GLuint a_pos_location_;
    GLuint a_tex_coord_location_;
    GLuint u_window_size_location_;
    GLuint u_texture_size_location_;
    GLuint u_camera_pos_location_;
    GLuint u_tile_size_location_;
    Vector2i window_size_;
    Vector2i framebuffer_size_;
    GLuint FBO_;
    GLuint framebuffer_texture_id_;
    TextureBufferBuilder screen_buffer_;

    friend class WorldRenderer;
};
