#pragma once

#include "SpriteManagerBase.hpp"
#include "TilesetSprite.hpp"

using TilesetSpriteId = int;

class TilesetSpriteManager : public SpriteManagerBase<TilesetSprite, TilesetSpriteId> {
public:
    TilesetSpriteId newSprite(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size);
    
    void setPos(TilesetSpriteId sprite_id, Vector2d pos);
    Vector2d getPos(TilesetSpriteId sprite_id);
    void move(TilesetSpriteId sprite_id, Vector2d move);
};
