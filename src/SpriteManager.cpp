#include "SpriteManager.hpp"
#include <cstddef>
#include <utility>

SpriteId SpriteManager::newSprite(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size) {
    for (size_t sprite_id = 0; sprite_id < sprites_.size(); sprite_id++) {
        auto &sprite = sprites_[sprite_id];
        if (!sprite.has_value()) {
            sprite.emplace(tileset, texture_pos, size);
            return sprite_id;
        }
    }
    sprites_.emplace_back(std::in_place, tileset, texture_pos, size);
    return sprites_.size() - 1;
}

void SpriteManager::destroySprite(SpriteId sprite_id) {
    sprites_.at(sprite_id).reset();
}

bool SpriteManager::ready() {
    for (auto &sprite : sprites_) {
        if (sprite.has_value() && !sprite->ready())
            return false;
    }
    
    return true;
}

void SpriteManager::forEachSprite(std::function<void(Sprite &sprite_id)> func) {
    for (auto &sprite : sprites_) {
        if (sprite.has_value())
            func(*sprite);
    }
}

void SpriteManager::setPos(SpriteId sprite_id, Vector2d pos) {
    sprites_.at(sprite_id)->pos_ = pos;
}

Vector2d SpriteManager::getPos(SpriteId sprite_id) {
    return sprites_.at(sprite_id)->pos_;
}

void SpriteManager::move(SpriteId sprite_id, Vector2d move) {
    auto &pos = sprites_.at(sprite_id)->pos_;
    pos = pos + move;
}
