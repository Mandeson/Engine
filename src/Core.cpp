#include "Core.hpp"
#include "renderer/SpriteRenderer.hpp"

Core::Core(ThreadPool &thread_pool, Font &default_font, Vector2i window_size)
        : thread_pool_(thread_pool), default_font_(default_font), window_size_(window_size),
        tileset_manager_(thread_pool), camera_pos_{0.0, 0.0} { }

void Core::windowSize(Vector2i window_size) {
    window_size_ = window_size;
}

void Core::render(PipelineState &pipeline_state, TextRenderer &text_renderer, TextureRenderer &texture_renderer, WorldRenderer &world_renderer) {
    if (map_ && map_->ready()) {
        Vector2d &camera_pos = camera_pos_;
        world_renderer.renderMap(texture_renderer, *map_, camera_pos);
        tileset_sprite_manager_.forEachObject([&world_renderer, &camera_pos] (TilesetSprite &sprite) {
            world_renderer.renderTilesetSprite(sprite, camera_pos);
        });
    }

    sprite_manager_.forEachObject([&pipeline_state, &texture_renderer, this] (Sprite &sprite) {
        if (sprite.ready())
            SpriteRenderer::render(texture_renderer, pipeline_state, window_size_, sprite);
    });

    text_manager_.forEachObject([&text_renderer] (TextObject &text_object) {
        text_renderer.render(text_object.getText(), text_object.getPos(), text_object.getColor());
    });
}

void Core::keyPressed(const std::string &key) {
    keyboard_controller_.keyPressed(key);
}

void Core::keyReleased(const std::string &key) {
    keyboard_controller_.keyReleased(key);
}

bool Core::ready() {
    return (!map_.has_value() || map_->ready()) && tileset_sprite_manager_.ready();
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

Font &Core::getDefaultFont() {
    return default_font_;
}

KeyboardController &Core::getKeyboardController() {
    return keyboard_controller_;
}

TilesetManager &Core::getTilesetManager() {
    return tileset_manager_;
}

TilesetSpriteManager &Core::getTilesetSpriteManager() {
    return tileset_sprite_manager_;
}

SpriteManager &Core::getSpriteManager() {
    return sprite_manager_;
}

TextManager &Core::getTextManager() {
    return text_manager_;
}

Map &Core::getMap() {
    if (!map_.has_value())
        throw std::runtime_error("Attempted to get the map which has not been loaded");
    return *map_;
}


ThreadPool &Core::getThreadPool() {
    return thread_pool_;
}

Vector2i Core::getWindowSize() {
    return window_size_;
}
