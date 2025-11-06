#pragma once

#include "../ObjectManagerBase.hpp"
#include "TilesetSprite.hpp"

using TilesetSpriteId = int;

class TilesetSpriteManager : public ObjectManagerBase<TilesetSprite, TilesetSpriteId> {
public:
    TilesetSpriteId newObject(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size);
    bool ready();
    
    void setPos(TilesetSpriteId sprite_id, Vector2d pos);
    Vector2d getPos(TilesetSpriteId sprite_id);
    void move(TilesetSpriteId sprite_id, Vector2d move);
};
