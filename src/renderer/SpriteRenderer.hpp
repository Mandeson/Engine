#pragma once

#include "TextureRenderer.hpp"
#include "../sprite/Sprite.hpp"

namespace SpriteRenderer {

void render(TextureRenderer &texture_renderer, PipelineState &pipeline_state, Vector2i window_size, Sprite &sprite);

}
