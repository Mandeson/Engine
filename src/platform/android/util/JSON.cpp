#include "../../../util/JSON.hpp"
#include "../asset_manager.hpp"

json JSON::parseFileImpl(const std::string &filename) {
    AAsset *asset = AAssetManager_open(getAssetManager(), filename.c_str(), AASSET_MODE_BUFFER);
    if (asset == nullptr)
        throw std::runtime_error(std::format("Cannot open asset: {}", filename));
    size_t size = AAsset_getLength(asset);
    std::string str(size, '\0');
    if (!AAsset_read(asset, &str[0], size)) {
        AAsset_close(asset);
        throw std::runtime_error(std::format("Cannot open asset: {}", filename));
    }
    AAsset_close(asset);
    return json::parse(str);
}
