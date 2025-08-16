#include "../../Texture.hpp"

void Texture::loadImpl(const std::string &filename) {
    std::string platform_filename = "assets/" + filename;
    upng_t *upng = upng_new_from_file(platform_filename.c_str());
    if (upng == nullptr || upng_get_error(upng) != UPNG_EOK)
        throw FileNotFoundError(std::move(platform_filename));
    decode(std::move(platform_filename), upng);
    upng_free(upng);
}
