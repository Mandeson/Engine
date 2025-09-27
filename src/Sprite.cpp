#include "Sprite.hpp"
#include "util/TextureRect.hpp"
#include "util/TextureRect.hpp"

Sprite::Sprite(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size)
        : tileset_(tileset), size_(size) {
    buffer_builder_.addRectangle(Vector2{0, 0}, size, TextureRect{texture_pos, size});
    buffer_builder_.end();
}

bool Sprite::ready() {
    return tileset_.getTexture().ready();
}
