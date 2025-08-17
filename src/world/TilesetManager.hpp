#pragma once

#include "Tileset.hpp"
#include <mutex>
#include <string>
#include <unordered_map>

class TilesetManager {
public:
    TilesetManager(ThreadPool &thread_pool);
    Tileset &getTileset(const std::string &filename);
private:
    ThreadPool &thread_pool_;
    std::unordered_map<std::string, Tileset> tilesets_;
    std::mutex mutex_;
};
