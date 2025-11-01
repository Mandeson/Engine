#pragma once

#include "SpriteBase.hpp"
#include "../util/Vector.hpp"
#include "../world/Tileset.hpp"

class TilesetSpriteManager;
class WorldRenderer;

class TilesetSprite : public SpriteBase {
public:
    TilesetSprite(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size);
    bool ready();
    Tileset &getTileset();
private:
    Tileset &tileset_;
};
