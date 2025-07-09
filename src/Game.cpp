#include "OpenGL.h"
#include "Game.hpp"
#include "util/Vector.hpp"

Game::Game(Vector2i window_size, int monitor_height, float uiScale, int randomSeed) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

Game::~Game() {

}

void Game::windowSize(Vector2i size) {
    glViewport(0, 0, size.x, size.y);
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Game::timeStep(double time) {

}
