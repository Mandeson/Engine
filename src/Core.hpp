#pragma once

#include "SpriteManager.hpp"
#include "ThreadPool.hpp"
#include "renderer/WorldRenderer.hpp"
#include "util/Vector.hpp"
#include "world/Map.hpp"
#include "world/TilesetManager.hpp"
#include "KeyboardController.hpp"

class Core {
public:
    Core(ThreadPool &thread_pool, Vector2i window_size);
    void windowSize(Vector2i window_size);
    void render(WorldRenderer &world_renderer);
    void keyPressed(const std::string &key);
    void keyReleased(const std::string &key);
    bool ready();
    void loadMap(const char *name);
    void setCameraPos(Vector2d pos);
    Vector2d getCameraPos();
    KeyboardController &getKeyboardController();
    TilesetManager &getTilesetManager();
    SpriteManager &getSpriteManager();
    Map &getMap();
private:
    ThreadPool &thread_pool_;
    KeyboardController keyboard_controller_;
    TilesetManager tileset_manager_;
    SpriteManager sprite_manager_;
    std::optional<Map> map_;
    Vector2d camera_pos_;
};
