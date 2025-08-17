#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace JSON {

json parseFileImpl(const std::string &filename);

}
