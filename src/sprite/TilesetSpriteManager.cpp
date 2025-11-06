#include "TilesetSpriteManager.hpp"

TilesetSpriteId TilesetSpriteManager::newObject(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size) {
    TilesetSpriteId id = findEmptyOrCreate();
    objects_[id].emplace(tileset, texture_pos, size);
    return id;
}

bool TilesetSpriteManager::ready() {
    for (auto &sprite : objects_)
        if (sprite.has_value() && !sprite->ready())
            return false;
    
    return true;
}

void TilesetSpriteManager::setPos(TilesetSpriteId sprite_id, Vector2d pos) {
    objects_.at(sprite_id)->setPos(pos);
}

Vector2d TilesetSpriteManager::getPos(TilesetSpriteId sprite_id) {
    return objects_.at(sprite_id)->getPos();
}

void TilesetSpriteManager::move(TilesetSpriteId sprite_id, Vector2d move) {
    auto &sprite = objects_.at(sprite_id).value();
    auto res = sprite.getPos() + move;
    sprite.setPos(res);
}