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

void SpriteManager::move(SpriteId sprite_id, Vector2d move) {
    auto &sprite = getObjectInstance(sprite_id);
    auto res = sprite.getPos() + move;
    sprite.setPos(res);
}

Sprite &SpriteManager::get(SpriteId sprite_id) {
    return getObjectInstance(sprite_id);
}
