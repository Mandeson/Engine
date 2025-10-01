#include "Engine.hpp"
#include <iostream>
#include "../../EngineContext.hpp"
#include "../../util/Logger.hpp"
#include <game-activity/GameActivityEvents.h>

void (*OpenGL::glBindFramebufferPtr)(GLenum, GLuint);
void (*OpenGL::glDeleteFramebuffersPtr)(GLsizei, const GLuint *);
void (*OpenGL::glGenFramebuffersPtr)(GLsizei, GLuint *);
GLenum (*OpenGL::glCheckFramebufferStatusPtr)(GLenum);
void (*OpenGL::glFramebufferTexture2DPtr)(GLenum, GLenum, GLenum, GLuint, GLint);

std::weak_ptr<Game> g_game;

static void loadDefaultOpenGLFramebufferPtrs() {
    OpenGL::glBindFramebufferPtr = glBindFramebuffer;
    OpenGL::glDeleteFramebuffersPtr = glDeleteFramebuffers;
    OpenGL::glGenFramebuffersPtr = glGenFramebuffers;
    OpenGL::glCheckFramebufferStatusPtr = glCheckFramebufferStatus;
    OpenGL::glFramebufferTexture2DPtr = glFramebufferTexture2D;
}

std::shared_ptr<Game> EngineContext::game() {
    if (g_game.expired())
        throw std::runtime_error("null Game pointer detected");
    return g_game.lock();
}

bool OpenGL::isGLES() {
	return true;
}

EngineImpl::EngineImpl(struct android_app *app)
   : app_(app),
     display_(EGL_NO_DISPLAY),
     surface_(EGL_NO_SURFACE),
     context_(EGL_NO_CONTEXT) { }

EngineImpl::~EngineImpl() {
    game_.reset();
    destroyContext();
    destroySurface();
    destroyDisplay();
}

static void handleCmdProxy(struct android_app *app, int32_t cmd) {
    auto *engine = reinterpret_cast<EngineImpl *>(app->userData);
    engine->handleCmd(cmd);
}

void EngineImpl::handleCmd(int32_t cmd) {
    switch(cmd) {
        case APP_CMD_INIT_WINDOW:
            has_window_ = true;
            break;
        case APP_CMD_TERM_WINDOW:
            destroySurface();
            has_window_ = false;
            break;
        default:
            break;
    }
}

void EngineImpl::readInput() {
    android_input_buffer *input_buffer = android_app_swap_input_buffers(app_);
    if (input_buffer == nullptr)
        return;
    android_app_clear_motion_events(input_buffer);

    static const std::unordered_map<int32_t, const char *> kKeys = {
            {AKEYCODE_W, "w"},
            {AKEYCODE_S, "s"},
            {AKEYCODE_A, "a"},
            {AKEYCODE_D, "d"},
            {AKEYCODE_DPAD_UP, "up"},
            {AKEYCODE_DPAD_DOWN, "down"},
            {AKEYCODE_DPAD_LEFT, "left"},
            {AKEYCODE_DPAD_RIGHT, "right"},
    };
    for (size_t i = 0; i < input_buffer->keyEventsCount; i++) {
        const GameActivityKeyEvent &event = input_buffer->keyEvents[i];
        if (kKeys.contains(event.keyCode)) {
            if (event.action == AKEY_EVENT_ACTION_DOWN)
                game_->keyEvent(kKeys.at(event.keyCode), Game::KeyState::kPress);
            else if (event.action == AKEY_EVENT_ACTION_UP)
                game_->keyEvent(kKeys.at(event.keyCode), Game::KeyState::kRelease);
        }
    }
    android_app_clear_key_events(input_buffer);
}

void EngineImpl::render() {
    if (display_ == EGL_NO_DISPLAY)
        if (!initDisplay())
            return;
    if (surface_ == EGL_NO_SURFACE)
        if (!initSurface())
            return;
    if (context_ == EGL_NO_CONTEXT)
        if (!initContext())
            return;

    EGLint surface_width, surface_height;
    eglQuerySurface(display_, surface_, EGL_WIDTH, &surface_width);
    eglQuerySurface(display_, surface_, EGL_HEIGHT, &surface_height);
    try {
        if (surface_width != surface_width_ || surface_height != surface_height_) {
            surface_width_ = surface_width;
            surface_height_ = surface_height;
            Vector2i window_size = {surface_width_, surface_height_};
            if (game_ == nullptr) {
                game_ = std::make_shared<Game>(window_size, surface_height_, 1.0f,
                                               time(nullptr));
                g_game = game_;
            } else {
                game_->windowSize(window_size);
            }
        }

        game_->render();
    } catch (std::exception &e) {
        Log::err(e.what());
        terminating_ = true;
        return;
    }

    if(eglSwapBuffers(display_, surface_) != EGL_TRUE) {
        Log::err("<Android> Error swapping buffers");
        terminating_ = true;
    }
}

void EngineImpl::gameLoop() {
    app_->userData = this;
    app_->onAppCmd = handleCmdProxy;

    std::optional<std::chrono::high_resolution_clock::time_point> time;
    do {
        bool done = false;
        while (!done) {
            int events;
            android_poll_source *poll_source;
            int result = ALooper_pollOnce(0, nullptr, &events, reinterpret_cast<void**>(&poll_source));
            switch (result) {
                case ALOOPER_POLL_TIMEOUT:
                    [[clang::fallthrough]];
                case ALOOPER_POLL_WAKE:
                    done = true;
                    break;
                case ALOOPER_EVENT_ERROR:
                    Log::err("<Android> ALooper_pollOnce error");
                    break;
                case ALOOPER_POLL_CALLBACK:
                    break;
                default:
                    if (poll_source)
                        poll_source->process(app_, poll_source);
            }
        }
        if (has_window_) {
            readInput();

            if (!time.has_value()) {
                time = std::chrono::high_resolution_clock::now();
            } else {
                auto now_time = std::chrono::high_resolution_clock::now();
                auto duration = now_time - time.value();
                double d_time = std::chrono::duration<double>(duration).count();
                if (d_time < kMaxFrameTime) {
                    while (d_time > kMaxTimeStep) {
                        game_->timeStep(kMaxTimeStep);
                        d_time -= kMaxTimeStep;
                    }
                    game_->timeStep(d_time);
                }
                time = now_time;
            }

            render();
        }
        if (terminating_) {
            GameActivity_finish(app_->activity);
            return;
        }
    } while (!app_->destroyRequested);
}

bool EngineImpl::initDisplay() {
    display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglInitialize(display_, nullptr, nullptr) == EGL_FALSE) {
        Log::err("<Android> Failed to init EGL display");
        terminating_ = true;
        return false;
    }
    Log::info("<Android> Initialized display");
    return true;
}

bool EngineImpl::initSurface() {
    EGLint written;
    constexpr EGLint surface_attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    eglChooseConfig(display_, surface_attribs, &config_, 1, &written);
    if (written != 1) {
        Log::err("<Android> Failed to get desired EGL config");
        terminating_ = true;
        return false;
    }
    surface_ = eglCreateWindowSurface(display_, config_, app_->window, nullptr);
    if (surface_ == EGL_NO_SURFACE) {
        Log::err("<Android> Failed to create EGL surface");
        terminating_ = true;
        return false;
    }

    // For debugging purposes: list all available surface configs
    EGLint num_configs;
    eglGetConfigs(display_, nullptr, 0, &num_configs);
    std::unique_ptr<EGLConfig[]> configs = std::make_unique<EGLConfig[]>(num_configs);
    eglGetConfigs(display_, configs.get(), num_configs, &num_configs);
    for (size_t i = 0; i < num_configs; i++) {
        EGLConfig config = *(configs.get() + i);
        EGLint red, green, blue, depth;
        if (eglGetConfigAttrib(display_, config, EGL_RED_SIZE, &red)
            && eglGetConfigAttrib(display_, config, EGL_GREEN_SIZE, &green)
            && eglGetConfigAttrib(display_, config, EGL_BLUE_SIZE, &blue)
            && eglGetConfigAttrib(display_, config, EGL_DEPTH_SIZE, &depth)) {
            Log::dbg("<Android> Found config with {}, {}, {}, {}", red, green, blue, depth);
        }
    }
    Log::info("<Android> Initialized surface");
    if (context_ != EGL_NO_CONTEXT) {
        if (eglMakeCurrent(display_, surface_, surface_, context_) == EGL_FALSE) {
            Log::err("<Android> eglMakeCurrent failed");
            terminating_ = true;
            return false;
        }
    }
    return true;
}

bool EngineImpl::initContext() {
    EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    context_ = eglCreateContext(display_, config_, nullptr, context_attribs);
    if (context_ == EGL_NO_CONTEXT) {
        Log::err("<Android> Failed to init EGL context");
        terminating_ = true;
        return false;
    }
    if (eglMakeCurrent(display_, surface_, surface_, context_) == EGL_FALSE) {
        Log::err("<Android> eglMakeCurrent failed");
        terminating_ = true;
        return false;
    }
    Log::info("<Android> Initialized context");
    loadDefaultOpenGLFramebufferPtrs();
    return true;
}

void EngineImpl::destroyDisplay() {
    if (display_ != EGL_NO_DISPLAY) {
        eglTerminate(display_);
        display_ = EGL_NO_DISPLAY;
        Log::info("<Android> Destroyed display");
    }
}

void EngineImpl::destroySurface() {
    if (surface_ != EGL_NO_SURFACE) {
        eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroySurface(display_, surface_);
        surface_ = EGL_NO_SURFACE;
        Log::info("<Android> Destroyed surface");
    }
}

void EngineImpl::destroyContext() {
    if (context_ != EGL_NO_CONTEXT) {
        eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(display_, context_);
        context_ = EGL_NO_CONTEXT;
        Log::info("<Android> Destroyed context");
    }
}
