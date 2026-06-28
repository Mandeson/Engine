#include "TilesetSpriteManager.hpp"
#include <algorithm>

bool TilesetSpriteManager::ready() {
    for (auto &sprite : objects_)
        if (sprite.has_value() && !sprite->instance.ready())
            return false;
    
    return true;
}

void TilesetSpriteManager::forEachVisibleSpriteDepthSorted(std::function<void(TilesetSprite &sprite)> func) {
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

void TilesetSpriteManager::setPos(TilesetSpriteId sprite_id, Vector2d pos) {
    getObjectInstance(sprite_id).setPos(pos);
}

void TilesetSpriteManager::setDepth(TilesetSpriteId sprite_id, double depth) {
    getObjectInstance(sprite_id).setDepth(depth);
}

Vector2d TilesetSpriteManager::getPos(TilesetSpriteId sprite_id) {
    return getObjectInstance(sprite_id).getPos();
}

void TilesetSpriteManager::move(TilesetSpriteId sprite_id, Vector2d move) {
    auto &sprite = getObjectInstance(sprite_id);
    auto res = sprite.getPos() + move;
    sprite.setPos(res);
}