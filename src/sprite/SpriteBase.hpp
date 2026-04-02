#pragma once

#include "../util/Vector.hpp"

class SpriteBase {
public:
    Vector2d getPos();
    Vector2i getSize();
    double getDepth();
    void setPos(Vector2d pos);
    void setDepth(double depth);
protected:
    Vector2d pos_ = {0, 0};
    double depth_ = 0; // for depth sorting
    Vector2i size_;
};
