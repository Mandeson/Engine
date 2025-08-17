#include "TilesetManager.hpp"
#include "Tileset.hpp"
#include <mutex>

TilesetManager::TilesetManager(ThreadPool &thread_pool) : thread_pool_(thread_pool) { }

Tileset &TilesetManager::getTileset(const std::string &filename) {
    std::scoped_lock lock{mutex_};
    auto &tileset = tilesets_.try_emplace(filename, thread_pool_, filename).first->second;
    return tileset;
}
