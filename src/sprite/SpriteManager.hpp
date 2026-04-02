#pragma once

#include "../ObjectManagerBase.hpp"
#include "Sprite.hpp"

using SpriteId = int;

class SpriteManager : public ObjectManagerBase<Sprite, SpriteId> {
public:
    SpriteId newObject(Texture &texture, TextureRect texture_rect);
    bool ready();
    void forEachSpriteDepthSorted(std::function<void(Sprite &sprite)> func);
    void setPos(SpriteId sprite_id, Vector2d pos);
    void setSize(SpriteId sprite_id, Vector2i size);
    void setDepth(SpriteId sprite_id, double depth);
    Vector2d getPos(SpriteId sprite_id);
    void move(SpriteId sprite_id, Vector2d move);
private:
    std::vector<size_t> temp_sort_;
};
