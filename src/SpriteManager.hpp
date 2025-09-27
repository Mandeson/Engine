#pragma once

#include "Sprite.hpp"
#include "util/Vector.hpp"
#include <functional>
#include <optional>
#include <vector>

using SpriteId = int;

class SpriteManager {
public:
    SpriteId newSprite(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size);
    void destroySprite(SpriteId sprite_id);
    bool ready();
    void forEachSprite(std::function<void(Sprite &sprite_id)> func);
    void setPos(SpriteId sprite_id, Vector2d pos);
    Vector2d getPos(SpriteId sprite_id);
    void move(SpriteId sprite_id, Vector2d move);
private:
    std::vector<std::optional<Sprite>> sprites_;
};
