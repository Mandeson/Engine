#include "JSON.hpp"
#include <format>

JSON::FileLoadingError::FileLoadingError(const std::string &filename)
        : message_(std::format("Error loading file: {}", filename)) { }

const char *JSON::FileLoadingError::what() const noexcept {
    return message_.c_str();
}
