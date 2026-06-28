#pragma once

#include "../ObjectManagerBase.hpp"
#include "Sprite.hpp"

using SpriteId = int;

class SpriteManager : public ObjectManagerBase<Sprite, SpriteId> {
public:
    bool ready();
    void forEachSpriteDepthSorted(std::function<void(Sprite &sprite)> func);
    void move(SpriteId sprite_id, Vector2d move);
    Sprite get(SpriteId sprite_id);
private:
    std::vector<size_t> temp_sort_;
};
