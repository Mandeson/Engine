#pragma once

#include <optional>
#include <string>
#include "../Texture.hpp"
#include "../util/TextureRect.hpp"

class Tileset {
public:
    Tileset(ThreadPool &thread_pool, const std::string &filename);

    Tileset(const Tileset &) = delete;
    Tileset& operator=(const Tileset &) = delete;

    bool ready();
    int getTileSize() const;
    TextureRect getTileTextureRect(int tile_id) const;
    Texture &getTexture();
private:
    int columns_;
    int tile_size_;
    std::optional<Texture> texture_;
};
