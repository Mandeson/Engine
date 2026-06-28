#pragma once

#include "TextureRenderer.hpp"
#include "../sprite/SpriteManager.hpp"
#include "../TextureBufferBuilder.hpp"

class SpriteRenderer {
public:
    SpriteRenderer() = default;
    ~SpriteRenderer() = default;
    SpriteRenderer(const SpriteRenderer &) = delete;
    SpriteRenderer &operator=(const SpriteRenderer &) = delete;

    void renderSprites(TextureRenderer &texture_renderer, PipelineState &pipeline_state,
            Vector2i window_size, SpriteManager &manager);
private:
    struct SpriteRenderUnit {
        Texture &texture;
        size_t quad_offset;
        size_t quad_count;
    };

    void renderSpriteUnit(TextureRenderer &texture_renderer, PipelineState &pipeline_state,
            Vector2i window_size, const SpriteRenderUnit &unit);
    
    TextureBufferBuilderFloat temp_sprite_buffer_;
    std::vector<SpriteRenderUnit> temp_render_units_;
};
