#include "Core.hpp"
#include "Sprite.hpp"
#include "SpriteManager.hpp"
#include "util/Vector.hpp"
#include "world/TilesetManager.hpp"
#include <stdexcept>

Core::Core(ThreadPool &thread_pool, Vector2i window_size) : thread_pool_(thread_pool),
        tileset_manager_(thread_pool), camera_pos_{0.0, 0.0} { }

void Core::windowSize(Vector2i window_size) {
    
}

void Core::render(WorldRenderer &world_renderer) {
    if (map_ && map_->ready()) {
        Vector2d &camera_pos = camera_pos_;
        world_renderer.renderMap(*map_, camera_pos);
        sprite_manager_.forEachSprite([&world_renderer, &camera_pos] (Sprite &sprite) {
            world_renderer.renderSprite(sprite, camera_pos);
        });
    }
}

void Core::keyPressed(const std::string &key) {
    keyboard_controller_.keyPressed(key);
}

void Core::keyReleased(const std::string &key) {
    keyboard_controller_.keyReleased(key);
}

bool Core::ready() {
    return (!map_.has_value() || map_->ready()) && sprite_manager_.ready();
}

void Core::loadMap(const char *name) {
    map_.emplace(thread_pool_, tileset_manager_, name);
}

void Core::setCameraPos(Vector2d pos) {
    camera_pos_ = pos;
}

Vector2d Core::getCameraPos() {
    return camera_pos_;
}

KeyboardController &Core::getKeyboardController() {
    return keyboard_controller_;
}

TilesetManager &Core::getTilesetManager() {
    return tileset_manager_;
}

SpriteManager &Core::getSpriteManager() {
    return sprite_manager_;
}

Map &Core::getMap() {
    if (!map_.has_value())
        throw std::runtime_error("Attempted to get the map which has not been loaded");
    return *map_;
}
