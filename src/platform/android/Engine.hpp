#pragma once

#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <EGL/egl.h>
#include <memory>
#include "../../Game.hpp"

class EngineImpl {
public:
    explicit EngineImpl(struct android_app *app);
    ~EngineImpl();
    void handleCmd(int32_t cmd);
    void readInput();
    void render();
    void gameLoop();
private:
    bool initDisplay();
    bool initSurface();
    bool initContext();
    void destroyDisplay();
    void destroySurface();
    void destroyContext();

    static constexpr double kMaxFrameTime = 0.5;
    static constexpr double kMaxTimeStep = 0.1;

    android_app *app_;
    bool has_window_ = false;
    EGLDisplay display_;
    EGLSurface surface_;
    EGLContext context_;
    EGLConfig config_ = nullptr;
    int surface_width_ = 0;
    int surface_height_ = 0;
    bool terminating_ = false;
    std::shared_ptr<Game> game_;
};
