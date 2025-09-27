#pragma once

#include "util/Vector.hpp"
#include "world/Tileset.hpp"
#include "TextureBufferBuilder.hpp"

class SpriteManager;
class WorldRenderer;

class Sprite {
public:
    Sprite(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size);
    bool ready();
private:
    Tileset &tileset_;
    Vector2i size_;
    TextureBufferBuilder buffer_builder_;
    Vector2d pos_ = {0, 0};

    friend class SpriteManager;
    friend class WorldRenderer;
};
