#include "TilesetSpriteManager.hpp"

TilesetSpriteId TilesetSpriteManager::newSprite(Tileset &tileset, Vector2<uint16_t> texture_pos, Vector2i size) {
    TilesetSpriteId id = findEmptyOrCreate();
    sprites_[id].emplace( tileset, texture_pos, size);
    return id;
}
