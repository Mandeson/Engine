#include "../../../lua/ScriptManager.hpp"
#include <string>
#include "../asset_manager.hpp"

void ScriptManager::doFileImpl(const std::string &filename) {
    auto path = ("scripts/" + filename);
    AAsset *asset = AAssetManager_open(getAssetManager(), path.c_str(), AASSET_MODE_BUFFER);
    if (asset == nullptr)
        throw FileNotFoundError(filename);
    size_t size = AAsset_getLength(asset);
    std::string str(size, '\0');
    if (!AAsset_read(asset, &str[0], size)) {
        AAsset_close(asset);
        throw FileNotFoundError(filename);
    }
    AAsset_close(asset);
    if (luaL_dostring(L_, &str[0]) != LUA_OK) {
        std::string error(lua_tostring(L_, -1));
        lua_pop(L_, lua_gettop(L_));
        throw ScriptError(std::move(error));
    }
}
