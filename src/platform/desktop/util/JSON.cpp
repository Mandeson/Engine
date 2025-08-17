#include "../../../util/JSON.hpp"
#include <fstream>

json JSON::parseFileImpl(const std::string &filename) {
    std::ifstream file("assets/" + filename);
    return json::parse(file);
}
