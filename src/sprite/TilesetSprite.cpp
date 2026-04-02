#include "TilesetSprite.hpp"

TilesetSprite::TilesetSprite(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size)
        : tileset_(tileset) {
    size_ = size;
    texture_rect_ = TextureRect{texture_pos, size};
}

bool TilesetSprite::ready() {
    return tileset_.getTexture().ready();
}

Tileset &TilesetSprite::getTileset() {
    return tileset_;
}

TextureRect TilesetSprite::getTextureRect() {
    return texture_rect_;
}
