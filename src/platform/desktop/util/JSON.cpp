#include "../../../util/JSON.hpp"
#include <fstream>

json JSON::parseFileImpl(const std::string &filename) {
    std::string path = "assets/" + filename;
    std::ifstream file(path);
    if (!file)
        throw FileLoadingError(path);
    return json::parse(file);
}
