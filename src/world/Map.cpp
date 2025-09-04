#include "Map.hpp"
#include "../util/JSON.hpp"
#include <memory>
#include <stdexcept>

Map::Chunk::Chunk(const std::map<int, size_t, std::greater<int>> &tileset_indexes, const std::vector<Tileset *> &tilesets, const json &data)
        : position(Vector2{data.at("x").template get<int>(), data.at("y").template get<int>()}) {
    if (data.at("height").template get<int>() != 16 || data.at("height").template get<int>() != 16)
            throw std::runtime_error("<Map::Chunk> Unsupported chunk size");
    Vector2i tile_pos = {0, 0};
    for (const auto &tile_id_data : data.at("data")) {
        int tile_code = tile_id_data.template get<int>();
        if (tile_code != 0) {
            bool horizontal_flip = tile_code & 0x80000000;
            bool vertical_flip = tile_code & 0x40000000;
            int tile_global_id = tile_code & 0x3FFFFFFF;
            auto tileset_it = tileset_indexes.upper_bound(tile_global_id + 1);
            if (tileset_it == tileset_indexes.end()) {
                Log::warn("<Map::Chunk> Wrong tile id: {}", tile_global_id);
            } else {
                size_t tileset_index = tileset_it->second;
                if (buffers.size() <= tileset_index)
                    buffers.resize(tileset_index + 1);
                auto &buffer_ptr = buffers[tileset_index];
                if (buffer_ptr == nullptr)
                    buffer_ptr = std::make_unique<TextureBufferBuilder>();
                int tile_tileset_id = tile_global_id -  tileset_it->first;
                auto tile_texture_rect = tilesets[tileset_index]->getTileTextureRect(tile_tileset_id);
                buffer_ptr->addRectangle(position + tile_pos, {1, 1}, tile_texture_rect, horizontal_flip, vertical_flip);
            }
        }
        tile_pos.x++;
        if (tile_pos.x == 16) {
            tile_pos.x = 0;
            tile_pos.y++;
        }
    }
    for (size_t tileset_index = 0; tileset_index < buffers.size(); tileset_index++) {
        auto &buffer = buffers[tileset_index];
        if (buffer != nullptr)
            buffer->end();
    }
}

Map::Layer::Layer(const std::map<int, size_t, std::greater<int>> &tileset_indexes, const std::vector<Tileset *> &tilesets, const json &data) {
    const auto &chunks_data = data.at("chunks");
    chunks.reserve(chunks_data.size());
    for (const auto &chunk_data : chunks_data)
        chunks.emplace_back(tileset_indexes, tilesets, chunk_data);
}

Map::Map(ThreadPool &thread_pool, TilesetManager &tileset_manager, const std::string &name)
        : background_work_active_(true) {
    thread_pool.execute([this, &tileset_manager, name]{
        Log::dbg("Loading map");
        try {
            json data = JSON::parseFileImpl("map/" + name + ".tmj");
            tile_size_ = data.at("tileheight").template get<int>();
            if (data.at("tilewidth").template get<int>() != tile_size_)
                throw std::runtime_error("<Map> Non-square tiles are not supported.");
            const auto &tilesets_data = data.at("tilesets");
            size_t tilesets_count = tilesets_data.size();
            tilesets_.resize(tilesets_count);
            std::map<int, size_t, std::greater<int>> tileset_indexes;
            for (size_t index = 0; index < tilesets_count; index++) {
                auto &tileset_data = tilesets_data.at(index);
                auto &tileset = tileset_manager.getTileset(tileset_data.at("source").template get<std::string>());
                if (tileset.getTileSize() != tile_size_)
                    throw std::runtime_error("<Map> Tileset and Map tile size incompatible");
                tilesets_[index] = &tileset;
                tileset_indexes[tileset_data.at("firstgid").template get<int>()] = index;
            }
            const auto &layers_data = data.at("layers");
            layers_.reserve(layers_data.size());
            for (const auto &layer_data : layers_data)
                layers_.emplace_back(tileset_indexes, tilesets_,layer_data);
        } catch (...) {
            exception_ = std::current_exception();
            error_ = true;
            {
                std::scoped_lock lock{mutex_};
                background_work_active_ = false;
            }
            background_work_condition_.notify_all();
            return;
        }
        Log::dbg("Loaded map");
        ready_ = true;
        {
            std::scoped_lock lock{mutex_};
            background_work_active_ = false;
        }
        background_work_condition_.notify_all();
    });
}

Map::~Map() {
    std::unique_lock lock(mutex_);
    background_work_condition_.wait(lock, [this]{
        return !background_work_active_;
    });
}

bool Map::ready() {
    if (error_)
        std::rethrow_exception(exception_);
    if (!ready_)
        return false;
    for (auto &tileset : tilesets_)
        if (!tileset->ready())
            return false;
    return true;
}

int Map::getTileSize() {
    return tile_size_;
}
