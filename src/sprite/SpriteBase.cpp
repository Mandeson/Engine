#include "SpriteBase.hpp"

Vector2d SpriteBase::getPos() {
    return pos_;
}

Vector2i SpriteBase::getSize() {
    return size_;
}

double SpriteBase::getDepth() {
    return depth_;
}

void SpriteBase::setPos(Vector2d pos) {
    pos_ = pos;
}

void SpriteBase::setDepth(double depth) {
    depth_ = depth;
}
