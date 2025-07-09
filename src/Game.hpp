#pragma once

#include "util/Vector.hpp"

class Game {
public:
    Game(Vector2i window_size, int monitor_height, float uiScale, int randomSeed);
    ~Game();
    void windowSize(Vector2i size);
    void render();
    void timeStep(double time);
private:

};
