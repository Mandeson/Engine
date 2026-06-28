#pragma once

#include <string>
#include <unordered_map>

class KeyboardController {
public:
    KeyboardController() = default;
    ~KeyboardController() = default;
    KeyboardController(const KeyboardController &) = delete;
    KeyboardController &operator=(const KeyboardController &) = delete;

    void keyPressed(const std::string &key);
    void keyReleased(const std::string &key);
    bool isKeyPressed(const std::string &key);
private:
    std::unordered_map<std::string, bool> key_states_;
};
