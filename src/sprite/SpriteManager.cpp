#include "SpriteManager.hpp"

#include <algorithm>

SpriteId SpriteManager::newObject(Texture &texture, TextureRect texture_rect) {
    SpriteId id = findEmptyOrCreate();
    objects_[id].emplace(texture, texture_rect);
    return id;
}

bool SpriteManager::ready() {
    for (auto &sprite : objects_)
        if (sprite.has_value() && !sprite->instance.ready())
            return false;
    
    return true;
}

void SpriteManager::forEachSpriteDepthSorted(std::function<void(Sprite &sprite)> func) {
    for (size_t index = 0; index < objects_.size(); index++)
        if (objects_[index].has_value())
            temp_sort_.push_back(index);

    std::sort(temp_sort_.begin(), temp_sort_.end(), [this] (size_t a_index, size_t b_index) {
        return objects_.at(a_index).value().instance.getDepth()
                > objects_.at(b_index).value().instance.getDepth();
    });

    for (size_t index : temp_sort_)
        func(objects_.at(index).value().instance);

    temp_sort_.clear();
}

void SpriteManager::setPos(SpriteId sprite_id, Vector2d pos) {
    objects_.at(sprite_id)->instance.setPos(pos);
}

void SpriteManager::setSize(SpriteId sprite_id, Vector2i size) {
    objects_.at(sprite_id)->instance.setSize(size);
}

void SpriteManager::setDepth(SpriteId sprite_id, double depth) {
    objects_.at(sprite_id)->instance.setDepth(depth);
}

Vector2d SpriteManager::getPos(SpriteId sprite_id) {
    return objects_.at(sprite_id)->instance.getPos();
}

void SpriteManager::move(SpriteId sprite_id, Vector2d move) {
    auto &sprite = objects_.at(sprite_id).value().instance;
    auto res = sprite.getPos() + move;
    sprite.setPos(res);
}
