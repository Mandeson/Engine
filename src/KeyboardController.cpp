#include "KeyboardController.hpp"

void KeyboardController::keyPressed(const std::string &key) {
    key_states_[key] = true;
}

void KeyboardController::keyReleased(const std::string &key) {
    key_states_[key] = false;
}

bool KeyboardController::isKeyPressed(const std::string &key) {
    return key_states_.contains(key) ? key_states_[key] : false;
}
