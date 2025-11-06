#include "SpriteManager.hpp"

SpriteId SpriteManager::newObject(Texture &texture, TextureRect texture_rect) {
    SpriteId id = findEmptyOrCreate();
    objects_[id].emplace(texture, texture_rect);
    return id;
}

bool SpriteManager::ready() {
    for (auto &sprite : objects_)
        if (sprite.has_value() && !sprite->ready())
            return false;
    
    return true;
}

void SpriteManager::setPos(SpriteId sprite_id, Vector2d pos) {
    objects_.at(sprite_id)->setPos(pos);
}

void SpriteManager::setSize(SpriteId sprite_id, Vector2i size) {
    objects_.at(sprite_id)->setSize(size);
}

Vector2d SpriteManager::getPos(SpriteId sprite_id) {
    return objects_.at(sprite_id)->getPos();
}

void SpriteManager::move(SpriteId sprite_id, Vector2d move) {
    auto &sprite = objects_.at(sprite_id).value();
    auto res = sprite.getPos() + move;
    sprite.setPos(res);
}
