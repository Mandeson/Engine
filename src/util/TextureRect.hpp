#pragma once

#include "Vector.hpp"
#include <cstdint>

struct TextureRect {
    Vector2<uint16_t> pos;
    Vector2<uint16_t> size;
};
