#include "../../Shader.hpp"
#include "asset_manager.hpp"

void Shader::loadSourceImpl(GLuint shader, std::string &filename) {
    AAsset *asset = AAssetManager_open(getAssetManager(), filename.c_str(), AASSET_MODE_BUFFER);
    if (asset == nullptr)
        throw Shader::FileNotFoundError(std::move(filename));
    size_t size = AAsset_getLength(asset);
    auto buffer = std::make_unique<GLchar[]>(size);
    if (!AAsset_read(asset, buffer.get(), size)) {
        AAsset_close(asset);
        throw Shader::FileNotFoundError(std::move(filename));
    }
    AAsset_close(asset);
    const GLchar *source = buffer.get();
    auto length = static_cast<GLint>(size);
    glShaderSource(shader, 1, &source, &length);
}
