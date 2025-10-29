#pragma once

#include "MapRenderer.hpp"
#include "../TilesetSprite.hpp"

class WorldRenderer {
public:
    WorldRenderer(PipelineState &pipeline_state, Vector2i window_size);
    void windowSize(Vector2i size);
    void renderMap(Map &map, Vector2d camera_pos);
    void renderTilesetSprite(TilesetSprite &sprite, Vector2d camera_pos);
    int getPixelScale();
private:
    PipelineState &pipeline_state_;
    int pixel_scale_ = 1;
    Vector2i window_size_;
    MapRenderer map_renderer_;
    Shader shader_;
    GLuint a_pos_location_;
    GLuint a_tex_coord_location_;
    GLuint u_window_size_location_;
    GLuint u_texture_size_location_;
    GLuint u_pos_location_;
    GLuint u_pixel_scale_location_;

    static constexpr int kPixelScaleDivider = 1080 / 6;
};
