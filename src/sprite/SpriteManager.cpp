#include "SpriteManager.hpp"
#include <algorithm>

bool SpriteManager::ready() {
    for (auto &sprite : objects_)
        if (sprite.has_value() && !sprite->instance.ready())
            return false;
    
    return true;
}

void SpriteManager::forEachSpriteDepthSorted(std::function<void(Sprite &sprite)> func) {
    for (size_t index = 0; index < objects_.size(); index++) {
        auto &sprite_opt = objects_[index];
        if (sprite_opt.has_value() && sprite_opt->visible)
            temp_sort_.push_back(index);
    }

    std::sort(temp_sort_.begin(), temp_sort_.end(), [this] (size_t a_index, size_t b_index) {
        return getObjectInstance(a_index).getDepth()
                > getObjectInstance(b_index).getDepth();
    });

    for (size_t index : temp_sort_)
        func(getObjectInstance(index));

    temp_sort_.clear();
}

void SpriteManager::setPos(SpriteId sprite_id, Vector2d pos) {
    getObjectInstance(sprite_id).setPos(pos);
}

void SpriteManager::setSize(SpriteId sprite_id, Vector2i size) {
    getObjectInstance(sprite_id).setSize(size);
}

void SpriteManager::setDepth(SpriteId sprite_id, double depth) {
    getObjectInstance(sprite_id).setDepth(depth);
}

Vector2d SpriteManager::getPos(SpriteId sprite_id) {
    return getObjectInstance(sprite_id).getPos();
}

void SpriteManager::move(SpriteId sprite_id, Vector2d move) {
    auto &sprite = getObjectInstance(sprite_id);
    auto res = sprite.getPos() + move;
    sprite.setPos(res);
}
