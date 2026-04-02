#include "TilesetSpriteManager.hpp"
#include <algorithm>

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

void TilesetSpriteManager::forEachSpriteDepthSorted(std::function<void(TilesetSprite &sprite)> func) {
    for (size_t index = 0; index < objects_.size(); index++)
        if (objects_[index].has_value())
            temp_sort_.push_back(index);

    std::sort(temp_sort_.begin(), temp_sort_.end(), [this] (size_t a_index, size_t b_index) {
        return objects_.at(a_index).value().getDepth() > objects_.at(b_index).value().getDepth();
    });

    for (size_t index : temp_sort_)
        func(objects_.at(index).value());

    temp_sort_.clear();
}

void TilesetSpriteManager::setPos(TilesetSpriteId sprite_id, Vector2d pos) {
    objects_.at(sprite_id)->setPos(pos);
}

void TilesetSpriteManager::setDepth(TilesetSpriteId sprite_id, double depth) {
    objects_.at(sprite_id)->setDepth(depth);
}

Vector2d TilesetSpriteManager::getPos(TilesetSpriteId sprite_id) {
    return objects_.at(sprite_id)->getPos();
}

void TilesetSpriteManager::move(TilesetSpriteId sprite_id, Vector2d move) {
    auto &sprite = objects_.at(sprite_id).value();
    auto res = sprite.getPos() + move;
    sprite.setPos(res);
}