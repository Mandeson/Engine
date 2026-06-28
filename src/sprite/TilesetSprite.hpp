#pragma once

#include "SpriteBase.hpp"
#include "../util/Vector.hpp"
#include "../world/Tileset.hpp"

class TilesetSpriteManager;
class WorldRenderer;

class TilesetSprite : public SpriteBase {
public:
    TilesetSprite(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size);

    ~TilesetSprite() = default;
    TilesetSprite(const TilesetSprite &) = delete;
    TilesetSprite(TilesetSprite &&) noexcept = default;
    TilesetSprite &operator=(const TilesetSprite &) = delete;
    TilesetSprite &operator=(TilesetSprite &&) = delete;

    bool ready();
    Tileset &getTileset();
    TextureRect getTextureRect();
private:
    Tileset &tileset_;
    TextureRect texture_rect_;
};
