#pragma once

#include "Game.hpp"
#include <memory>

namespace EngineContext {
    std::shared_ptr<Game> game(); // Defined in platform/...
    std::shared_ptr<Core> core(); // Defined in game.cpp
}
