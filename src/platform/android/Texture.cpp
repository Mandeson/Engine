#include "../../Texture.hpp"
#include "asset_manager.hpp"

void Texture::loadImpl(const std::string &filename) {
    std::string platform_filename = filename;
    AAsset *asset = AAssetManager_open(getAssetManager(), platform_filename.c_str(), AASSET_MODE_BUFFER);
    if (asset == nullptr)
        throw Texture::FileNotFoundError(std::move(platform_filename));
    size_t size = AAsset_getLength(asset);
    auto buffer = std::make_unique<uint8_t[]>(size);
    if (!AAsset_read(asset, buffer.get(), size)) {
        AAsset_close(asset);
        throw Texture::FileNotFoundError(std::move(platform_filename));
    }
    AAsset_close(asset);
    upng_t *upng = upng_new_from_bytes(buffer.get(), size);
    if (upng == nullptr)
        throw FileNotFoundError(std::move(platform_filename));
    if (upng_get_error(upng) != UPNG_EOK) {
        upng_free(upng);
        throw FileNotFoundError(std::move(platform_filename));
    }
    decode(std::move(platform_filename), upng);
    upng_free(upng);
}
