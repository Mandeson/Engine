#pragma once

#include "ThreadPool.hpp"
#include "renderer/WorldRenderer.hpp"
#include "util/Vector.hpp"
#include "world/Map.hpp"
#include "world/TilesetManager.hpp"

class Core {
public:
    Core(ThreadPool &thread_pool, Vector2i window_size);
    void windowSize(Vector2i window_size);
    void render(WorldRenderer &world_renderer);
    void loadMap(const char *name);
private:
    ThreadPool &thread_pool_;
    TilesetManager tileset_manager_;
    std::optional<Map> map_;
};
