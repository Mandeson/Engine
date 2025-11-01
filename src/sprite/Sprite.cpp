#include "Sprite.hpp"

Sprite::Sprite(Texture &texture, TextureRect texture_rect) : texture_(texture) {
    size_ = texture_rect.size;
    buffer_builder_.addRectangle(Vector2{0, 0}, Vector2{1, 1}, texture_rect);
    buffer_builder_.end();
}

bool Sprite::ready() {
    return texture_.ready();
}

Texture &Sprite::getTexture() {
    return texture_;
}

void Sprite::setSize(Vector2i size) {
    size_ = size;
}
