#pragma once

#include <exception>
#include <memory>
#include <nlohmann/json.hpp>
#include <vector>
#include "../ThreadPool.hpp"
#include "Tileset.hpp"
#include "TilesetManager.hpp"
#include "../util/Vector.hpp"
#include "../TextureBufferBuilder.hpp"
using json = nlohmann::json;

class MapRenderer;

class Map {
public:
    struct Chunk {
        Chunk(const std::map<int, size_t, std::greater<int>> &tileset_indexes, const std::vector<Tileset *> &tilesets, const json &data);

        Vector2<int16_t> position;
        std::vector<std::unique_ptr<TextureBufferBuilder>> buffers;
    };

    struct Layer {
        Layer(const std::map<int, size_t, std::greater<int>> &tileset_indexes, const std::vector<Tileset *> &tilesets, const json &data);

        std::vector<Chunk> chunks;
    };

    Map(ThreadPool &thread_pool, TilesetManager &tileset_manager, const std::string &name);

    Map(const Map &) = delete;
    Map& operator=(const Map &) = delete;

    ~Map();

    bool ready();
    int getTileSize();
private:
    bool background_work_active_;
    std::mutex mutex_;
    std::condition_variable background_work_condition_;
    std::atomic<bool> error_ = false;
    std::exception_ptr exception_ = nullptr;
    std::atomic<bool> ready_ = false;
    int tile_size_;
    std::vector<Tileset *> tilesets_;
    std::vector<Layer> layers_;

    friend class MapRenderer;
};
