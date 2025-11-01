#pragma once

#include "PipelineState.hpp"
#include "ThreadPool.hpp"
#include "sprite/TilesetSpriteManager.hpp"
#include "sprite/SpriteManager.hpp"
#include "renderer/WorldRenderer.hpp"
#include "renderer/TextureRenderer.hpp"
#include "util/Vector.hpp"
#include "world/Map.hpp"
#include "world/TilesetManager.hpp"
#include "KeyboardController.hpp"

class Core {
public:
    Core(ThreadPool &thread_pool, Vector2i window_size);
    void windowSize(Vector2i window_size);
    void render(PipelineState &pipeline_state, TextureRenderer &texture_renderer, WorldRenderer &world_renderer);
    void keyPressed(const std::string &key);
    void keyReleased(const std::string &key);
    bool ready();
    void loadMap(const char *name);
    void setCameraPos(Vector2d pos);
    Vector2d getCameraPos();
    KeyboardController &getKeyboardController();
    TilesetManager &getTilesetManager();
    TilesetSpriteManager &getTilesetSpriteManager();
    SpriteManager &getSpriteManager();
    Map &getMap();
    ThreadPool &getThreadPool();
private:
    ThreadPool &thread_pool_;
    Vector2i window_size_;
    KeyboardController keyboard_controller_;
    TilesetManager tileset_manager_;
    TilesetSpriteManager tileset_sprite_manager_;
    SpriteManager sprite_manager_;
    std::optional<Map> map_;
    Vector2d camera_pos_;
};
