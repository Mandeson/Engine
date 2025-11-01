#include "Tileset.hpp"
#include "../util/JSON.hpp"

Tileset::Tileset(ThreadPool &thread_pool, const std::string &filename) {
    json data = JSON::parseFileImpl("map/" + filename);
    tile_size_ = data.at("tileheight").template get<int>();
    if (data.at("tilewidth").template get<int>() != tile_size_)
        throw std::runtime_error("<Tileset> Non-square tiles are not supported.");
    texture_.emplace(thread_pool, "map/" + data.at("image").template get<std::string>(), Texture::Filtering::kNearest);
    columns_ = data.at("columns").template get<int>();
}

bool Tileset::ready() {
    return texture_->ready();
}

int Tileset::getTileSize() const {
    return tile_size_;
}

TextureRect Tileset::getTileTextureRect(int tile_id) const {
    return {Vector2{tile_id % columns_, tile_id / columns_} * tile_size_,Vector2{tile_size_, tile_size_}};
}

Texture &Tileset::getTexture() {
    return texture_.value();
}
