#pragma once

#include "../util/Vector.hpp"
#include "../TextureBufferBuilder.hpp"

class SpriteBase {
public:
    TextureBufferBuilder &getBuffer();
    Vector2d getPos();
    Vector2i getSize();
    void setPos(Vector2d pos);
protected:
    TextureBufferBuilder buffer_builder_;
    Vector2d pos_ = {0, 0};
    Vector2i size_;
};
