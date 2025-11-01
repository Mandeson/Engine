#include "SpriteManager.hpp"

SpriteId SpriteManager::newSprite(Texture &texture, TextureRect texture_rect) {
    SpriteId id = findEmptyOrCreate();
    sprites_[id].emplace(texture, texture_rect);
    return id;
}

void SpriteManager::setPos(SpriteId sprite_id, Vector2d pos) {
    sprites_.at(sprite_id)->setPos(pos);
}

void SpriteManager::setSize(SpriteId sprite_id, Vector2i size) {
    sprites_.at(sprite_id)->setSize(size);
}

Vector2d SpriteManager::getPos(SpriteId sprite_id) {
    return sprites_.at(sprite_id)->getPos();
}

void SpriteManager::move(SpriteId sprite_id, Vector2d move) {
    auto &sprite = sprites_.at(sprite_id).value();
    auto res = sprite.getPos() + move;
    sprite.setPos(res);
}
