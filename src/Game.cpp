#include "Game.hpp"
#include <lua.hpp>
#include "DebugDisplay.hpp"
#include "EngineContext.hpp"
#include "OpenGL.hpp"
#include "util/Logger.hpp"

std::weak_ptr<Core> g_core;

Game::Game(Vector2i window_size, int monitor_height, float ui_scale, int random_seed)
        : ui_scale_(ui_scale), window_size_(window_size), font_("Roboto-Regular.ttf"),
        error_text_(thread_pool_, font_, ui_scale * 18.0f), debug_display_(thread_pool_, font_, ui_scale),
        text_renderer_(pipeline_state_, window_size) {
    try {
        core_ = std::make_shared<Core>(thread_pool_, window_size);
        g_core = core_;
        script_manager_.loadMainScript();
        script_manager_.initApiCall();
        
        world_renderer_.emplace(pipeline_state_, window_size);
        texture_renderer_.emplace();
    } catch (std::exception &e) {
        error_ = true;
        buildErrorMessage(e.what());
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

Game::~Game() {
    
}

void Game::windowSize(Vector2i size) {
    window_size_ = size;
    glViewport(0, 0, size.x, size.y);
    text_renderer_.windowSize(size);
    if(!error_) {
        world_renderer_->windowSize(size);
        core_->windowSize(size);
    } else {
        buildErrorMessage();
    }
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    if (!error_) {
        try {
            core_->render(pipeline_state_, *texture_renderer_, *world_renderer_);
            debug_display_.render(text_renderer_);
        } catch (std::exception &e) {
            error_ = true;
            buildErrorMessage(e.what());
        }
    } else {
        text_renderer_.render(error_text_, static_cast<Vector2i>(Vector2{kErrorTextMargin, kErrorTextMargin} * ui_scale_),
                Color{255, 0, 0});
    }
}

void Game::timeStep(double time) {
    if (error_ || !core_->ready())
        return;

    try {
        debug_display_.timeStep(time);
        script_manager_.timeStepApiCall(time);
    } catch (std::exception &e) {
        error_ = true;
        buildErrorMessage(e.what());
    }
}

void Game::keyEvent(const std::string &key, KeyState state) {
    if (error_)
        return;

    try {
        if (state == Game::KeyState::kPress) {
            core_->keyPressed(key);
            script_manager_.keyPressedApiCall(key);
        } else {
            core_->keyReleased(key);
            script_manager_.keyReleasedApiCall(key);
        }
    } catch (std::exception &e) {
        error_ = true;
        buildErrorMessage(e.what());
    }
}

void Game::touchEvent(Game::PointerAction action, Vector2f pos, int pointer_id) {
    if (error_)
        return;

    try {
        if (action == PointerAction::kDown)
            script_manager_.pointerDownApiCall(pos, pointer_id);
        else if (action == PointerAction::kUp)
            script_manager_.pointerUpApiCall(pos, pointer_id);
        else
            script_manager_.pointerMoveApiCall(pos, pointer_id);
    } catch (std::exception &e) {
        error_ = true;
        buildErrorMessage(e.what());
    }
}

void Game::touchEventCancel() {
    if (error_)
        return;

    try {
        script_manager_.pointerCancelApiCall();
    } catch (std::exception &e) {
        error_ = true;
        buildErrorMessage(e.what());
    }
}

Font &Game::getDefaultFont() {
    return font_;
}

WorldRenderer &Game::getWorldRenderer() {
    return world_renderer_.value();
}

Vector2i Game::getWindowSize() {
    return window_size_;
}

std::shared_ptr<Core> EngineContext::core() {
    if (g_core.expired())
        throw std::runtime_error("Null pointer detected by EngineContext::core()");
    return g_core.lock();
}

void Game::buildErrorMessage(const char *message) {
    if (message) {
        error_message_ = std::format("Error:\n{}", message);
        Log::err(error_message_);
    }
    error_text_.setString(error_message_, Text::Alignment::kLeft,
            static_cast<int>(window_size_.x - kErrorTextMargin * ui_scale_ * 2));
}
