#include "Sprite.hpp"

Sprite::Sprite(Texture &texture, TextureRect texture_rect) : texture_(texture),
        texture_rect_(texture_rect) {
    size_ = texture_rect.size;
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

TextureRect Sprite::getTextureRect() {
    return texture_rect_;
}

