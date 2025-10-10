#include "../../../lua/ScriptManager.hpp"
#include <string>

static void doFile(lua_State *L, const std::string &filename) {
    std::string path = "assets/scripts/" + filename;
    int top = lua_gettop(L);
    int status = luaL_loadfile(L, path.c_str());
    if (status == LUA_ERRFILE) {
        lua_settop(L, top); // we don't need the error information, we know it's a file error and know its path
        throw ScriptManager::FileNotFoundError(path);
    }
    if (status != LUA_OK || lua_pcall(L, 0, LUA_MULTRET, 0) != LUA_OK) {
        // after calling lua_pop the string may be garbage collected, so we need to copy it
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
