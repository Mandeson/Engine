#include "SpriteBase.hpp"

TextureBufferBuilder &SpriteBase::getBuffer() {
    return buffer_builder_;
}

Vector2d SpriteBase::getPos() {
    return pos_;
}

Vector2i SpriteBase::getSize() {
    return size_;
}

void SpriteBase::setPos(Vector2d pos) {
    pos_ = pos;
}
