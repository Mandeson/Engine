#pragma once

#include "../ObjectManagerBase.hpp"
#include "TilesetSprite.hpp"

using TilesetSpriteId = int;

class TilesetSpriteManager : public ObjectManagerBase<TilesetSprite, TilesetSpriteId> {
public:
    bool ready();
    void forEachVisibleSpriteDepthSorted(std::function<void(TilesetSprite &sprite)> func);
    void move(TilesetSpriteId sprite_id, Vector2d move);
    TilesetSprite &get(TilesetSpriteId sprite_id);
private:
    std::vector<size_t> temp_sort_;
};
