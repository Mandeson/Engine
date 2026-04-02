#pragma once

#include "MapRenderer.hpp"
#include "../sprite/TilesetSpriteManager.hpp"
#include <vector>

class WorldRenderer {
public:
    WorldRenderer(PipelineState &pipeline_state, Vector2i window_size);
    void windowSize(Vector2i size);
    void renderMap(TextureRenderer &texture_renderer, Map &map, Vector2d camera_pos);
    void renderTilesetSprites(TilesetSpriteManager &manager, Vector2d camera_pos);
    int getPixelScale();
private:
    struct SpriteRenderUnit {
        Texture &texture;
        size_t quad_offset;
        size_t quad_count;
    };

    void renderSpriteUnit(const SpriteRenderUnit &unit);

    PipelineState &pipeline_state_;
    TextureBufferBuilderFloat temp_sprite_buffer_;
    std::vector<SpriteRenderUnit> temp_render_units_;
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

    static constexpr int kPixelScaleDivider = 1080 / 4;
};
