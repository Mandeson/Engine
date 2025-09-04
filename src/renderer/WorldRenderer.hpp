#pragma once

#include "MapRenderer.hpp"
class WorldRenderer {
public:
    WorldRenderer(Vector2i window_size);
    void windowSize(Vector2i size);
    void render(Map &map, Vector2f camera_pos);
private:
    void setWindowSize(Vector2i size);

    MapRenderer map_renderer_;
    Shader shader_;
    GLuint a_pos_location_;
    GLuint a_tex_coord_location_;
    GLuint u_window_size_location_;
    GLuint u_texture_size_location_;
    GLuint u_pos_location_;
    GLuint u_pixel_scale_location_;
};
