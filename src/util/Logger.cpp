#include "Logger.hpp"

void Log::info(const std::string &str) {
    Log::printLineInfoImpl(str);
}

void Log::err(const std::string &str) {
    Log::printLineInfoImpl(str);
}

void Log::warn(const std::string &str) {
    Log::printLineInfoImpl(str);
}

void Log::dbg([[maybe_unused]] const std::string &str) {
#ifdef ENGINE_DEBUG
    Log::printLineInfoImpl(str);
#endif
}
