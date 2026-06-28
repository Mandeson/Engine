#pragma once

#include "../ObjectManagerBase.hpp"
#include "TilesetSprite.hpp"

using TilesetSpriteId = int;

class TilesetSpriteManager : public ObjectManagerBase<TilesetSprite, TilesetSpriteId> {
public:
    bool ready();
    void forEachSpriteDepthSorted(std::function<void(TilesetSprite &sprite)> func);
    void setPos(TilesetSpriteId sprite_id, Vector2d pos);
    void setDepth(TilesetSpriteId sprite_id, double depth);
    Vector2d getPos(TilesetSpriteId sprite_id);
    void move(TilesetSpriteId sprite_id, Vector2d move);
private:
    std::vector<size_t> temp_sort_;
};
