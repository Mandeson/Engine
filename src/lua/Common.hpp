#pragma once

#include <string_view>

namespace Lua::Common {

std::string methodCallOnDestroyedObjectErrorText(std::string_view objectType, std::string_view methodName);

}
