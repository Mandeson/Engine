#include "Core.hpp"

Core::Core(ThreadPool &thread_pool, Vector2i window_size) : thread_pool_(thread_pool),
        tileset_manager_(thread_pool) { }

void Core::windowSize(Vector2i window_size) {
    
}

void Core::render(WorldRenderer &world_renderer) {
    if (map_ && map_->ready()) {
        world_renderer.render(*map_, Vector2{-5, -5});
    }
}

void Core::loadMap(const char *name) {
    map_.emplace(thread_pool_, tileset_manager_, name);
}
