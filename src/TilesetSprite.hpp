#pragma once

#include "util/Vector.hpp"
#include "world/Tileset.hpp"
#include "TextureBufferBuilder.hpp"

template <typename T, typename I>
class SpriteManagerBase;
class WorldRenderer;

class TilesetSprite {
public:
    TilesetSprite(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size);
    bool ready();
private:
    Tileset &tileset_;
    Vector2i size_;
    TextureBufferBuilder buffer_builder_;
    Vector2d pos_ = {0, 0};

    template <typename T, typename I>
    friend class SpriteManagerBase;
    
    friend class WorldRenderer;
};
