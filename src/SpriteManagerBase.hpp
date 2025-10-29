#pragma once

#include "util/Vector.hpp"
#include <functional>
#include <optional>
#include <vector>

// T - sprite class
// I - sprite id (usually int)
template <typename T, typename I>
class SpriteManagerBase {
public:
    void destroySprite(I sprite_id) {
        sprites_.at(sprite_id).reset();
    }

    bool ready() {
        for (auto &sprite : sprites_)
            if (sprite.has_value() && !sprite->ready())
                return false;
        
        return true;
    }

    void forEachSprite(std::function<void(T &sprite_id)> func) {
        for (auto &sprite : sprites_)
            if (sprite.has_value())
                func(*sprite);
    }

    void setPos(I sprite_id, Vector2d pos) {
        sprites_.at(sprite_id)->pos_ = pos;
    }

    Vector2d getPos(I sprite_id) {
        return sprites_.at(sprite_id)->pos_;
    }

    void move(I sprite_id, Vector2d move) {
        auto &pos = sprites_.at(sprite_id)->pos_;
        pos = pos + move;
    }
protected:
    I findEmptyOrCreate() {
        for (size_t sprite_id = 0; sprite_id < sprites_.size(); sprite_id++) {
            auto &sprite = sprites_[sprite_id];
            if (!sprite.has_value())
                return sprite_id;
        }
        
        sprites_.emplace_back();
        return sprites_.size() - 1; // Return the last, newly added element
    }

    std::vector<std::optional<T>> sprites_;
};
