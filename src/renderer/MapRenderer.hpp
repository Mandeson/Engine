#pragma once

#include "../Shader.hpp"
#include "../world/Map.hpp"

class MapRenderer {
public:
    MapRenderer(Vector2i window_size);
    void windowSize(Vector2i size);
    void render(Map &map, Vector2f camera_pos);
private:
    Shader shader_;
    GLuint a_pos_location_;
    GLuint a_tex_coord_location_;
    GLuint u_window_size_location_;
    GLuint u_texture_size_location_;
    GLuint u_camera_pos_location_;
    GLuint u_tile_size_location_;
};
