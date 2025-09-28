#pragma once

#include <memory>
#include <lua.hpp>
#include "Core.hpp"
#include "DebugDisplay.hpp"
#include "ThreadPool.hpp"
#include "lua/ScriptManager.hpp"
#include "renderer/TextRenderer.hpp"
#include "renderer/WorldRenderer.hpp"
#include "text/Font.hpp"
#include "util/Vector.hpp"

class Game {
public:
    enum class KeyState {
        kPress,
        kRelease
    };

    Game(Vector2i window_size, int monitor_height, float ui_scale, int random_seed);
    ~Game();
    void windowSize(Vector2i size);
    void render();
    void timeStep(double time);
    void keyEvent(const std::string &key, KeyState state);
    ThreadPool &getThreadPool();
    Font &getDefaultFont();
    WorldRenderer &getWorldRenderer();
private:
    void buildErrorMessage(const char *message = nullptr);

    float ui_scale_;
    Vector2i window_size_;
    Font font_;
    ThreadPool thread_pool_;
    bool error_ = false;
    Text error_text_;
    std::string error_message_;
    DebugDisplay debug_display_;
    TextRenderer text_renderer_;
    std::shared_ptr<Core> core_;
    std::optional<WorldRenderer> world_renderer_;
    ScriptManager script_manager_;

    static constexpr int kErrorTextMargin = 64;
};
