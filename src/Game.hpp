#pragma once

#include <lua.hpp>
#include "text/Font.hpp"
#include "util/Vector.hpp"

class Game {
public:
    Game(Vector2i window_size, int monitor_height, float ui_scale, int random_seed);
    ~Game();
    void windowSize(Vector2i size);
    void render();
    void timeStep(double time);
private:
    Font font_;
    lua_State *L;
};
