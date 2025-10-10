#include "../../../lua/ScriptManager.hpp"
#include <string>
#include "../asset_manager.hpp"

static void doFile(lua_State *L, const std::string &filename) {
    auto path = ("scripts/" + filename);
    AAsset *asset = AAssetManager_open(getAssetManager(), path.c_str(), AASSET_MODE_BUFFER);
    if (asset == nullptr)
        throw ScriptManager::FileNotFoundError(filename);
    size_t size = AAsset_getLength(asset);
    std::string str(size, '\0');
    if (!AAsset_read(asset, &str[0], size)) {
        AAsset_close(asset);
        throw ScriptManager::FileNotFoundError(filename);
    }
    AAsset_close(asset);

    int top = lua_gettop(L);
    if (luaL_dostring(L, &str[0]) != LUA_OK) {
        std::string error(lua_tostring(L, -1));
        lua_settop(L, top);
        throw ScriptManager::ScriptError(std::move(error));
    }
}

void ScriptManager::doFileImpl(const std::string &filename) {
    int top = lua_gettop(L_);
    checkStack(top);
    doFile(L_, filename);
}

static int dofileLua(lua_State *L) noexcept {
    const char *filename = luaL_checkstring(L, 1);
    try {
        doFile(L, filename);
    } catch (std::exception &e) {
        luaL_error(L, e.what());
    }
    return lua_gettop(L) - 1; // Do not return the file name, only the values returned by the script
}

void ScriptManager::registerImpl() {
    lua_pushcfunction(L_, dofileLua);
    lua_setglobal(L_, "dofile");
}
