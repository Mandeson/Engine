#include "OpenGL.hpp"
#include "text/Font.hpp"
#include "util/Logger.hpp"
#include "util/Vector.hpp"
#include "Game.hpp"

Game::Game(Vector2i window_size, int monitor_height, float ui_scale, int random_seed)
        : font_("Roboto-Regular.ttf") {
    L = luaL_newstate();

    lua_newtable(L);
    lua_setglobal(L, "Engine");
    Font::registerLua(L, &font_);

    luaL_openlibs(L);
    if (luaL_dofile(L, "assets/scripts/game.lua") != LUA_OK) {
        Log::err("Lua: {}", lua_tostring(L, -1));
        return;
    }

    // GPU
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

Game::~Game() {
    lua_close(L);
}

void Game::windowSize(Vector2i size) {
    glViewport(0, 0, size.x, size.y);
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Game::timeStep(double time) {

}
