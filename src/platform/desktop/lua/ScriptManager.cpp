#include "../../../lua/ScriptManager.hpp"
#include <string>

void ScriptManager::doFileImpl(const std::string &filename) {
    std::string path = "assets/scripts/" + filename;
    int status = luaL_loadfile(L_, path.c_str());
    if (status == LUA_ERRFILE) {
        lua_pop(L_, lua_gettop(L_)); // we don't need the error information, we know it's a file error and know its path
        throw FileNotFoundError(path);
    }
    if (status != LUA_OK || lua_pcall(L_, 0, LUA_MULTRET, 0) != LUA_OK) {
        // after calling lua_pop the string may be garbage collected, so we need to copy it
        std::string error(lua_tostring(L_, -1));
        lua_pop(L_, lua_gettop(L_));
        throw ScriptError(std::move(error));
    }
}
