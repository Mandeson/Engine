#include "Common.hpp"
#include <format>

std::string Lua::Common::methodCallOnDestroyedObjectErrorText(std::string_view objectType, std::string_view methodName) {
    return std::format("Tried to call method '{}' on a destroyed {} object", methodName, objectType);
}
