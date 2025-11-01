#include "TilesetSpriteManager.hpp"

TilesetSpriteId TilesetSpriteManager::newSprite(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size) {
    TilesetSpriteId id = findEmptyOrCreate();
    sprites_[id].emplace(tileset, texture_pos, size);
    return id;
}

void TilesetSpriteManager::setPos(TilesetSpriteId sprite_id, Vector2d pos) {
    sprites_.at(sprite_id)->setPos(pos);
}

Vector2d TilesetSpriteManager::getPos(TilesetSpriteId sprite_id) {
    return sprites_.at(sprite_id)->getPos();
}

void TilesetSpriteManager::move(TilesetSpriteId sprite_id, Vector2d move) {
    auto &sprite = sprites_.at(sprite_id).value();
    auto res = sprite.getPos() + move;
    sprite.setPos(res);
}