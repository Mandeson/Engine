#pragma once

#include "PipelineState.hpp"
#include "TextManager.hpp"
#include "ThreadPool.hpp"
#include "renderer/TextRenderer.hpp"
#include "sprite/TilesetSpriteManager.hpp"
#include "sprite/SpriteManager.hpp"
#include "renderer/WorldRenderer.hpp"
#include "renderer/TextureRenderer.hpp"
#include "text/Font.hpp"
#include "util/Vector.hpp"
#include "world/Map.hpp"
#include "world/TilesetManager.hpp"
#include "KeyboardController.hpp"

class Core {
public:
    Core(ThreadPool &thread_pool, Font &default_font_, Vector2i window_size);
    void windowSize(Vector2i window_size);
    void render(PipelineState &pipeline_state, TextRenderer &text_renderer, TextureRenderer &texture_renderer, WorldRenderer &world_renderer);
    void keyPressed(const std::string &key);
    void keyReleased(const std::string &key);
    bool ready();
    void loadMap(const char *name);
    void setCameraPos(Vector2d pos);
    Vector2d getCameraPos();
    Font &getDefaultFont();
    KeyboardController &getKeyboardController();
    TilesetManager &getTilesetManager();
    TilesetSpriteManager &getTilesetSpriteManager();
    SpriteManager &getSpriteManager();
    TextManager &getTextManager();
    Map &getMap();
    ThreadPool &getThreadPool();
    Vector2i getWindowSize();
private:
    ThreadPool &thread_pool_;
    Font &default_font_;
    Vector2i window_size_;
    KeyboardController keyboard_controller_;
    TilesetManager tileset_manager_;
    TilesetSpriteManager tileset_sprite_manager_;
    SpriteManager sprite_manager_;
    TextManager text_manager_;
    std::optional<Map> map_;
    Vector2d camera_pos_;
};
