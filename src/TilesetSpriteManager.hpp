#pragma once

#include "SpriteManagerBase.hpp"
#include "TilesetSprite.hpp"

using TilesetSpriteId = int;

class TilesetSpriteManager : public SpriteManagerBase<TilesetSprite, TilesetSpriteId> {
public:
    TilesetSpriteId newSprite(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size);
};
