#include "ScriptManager.hpp"
#include <format>
#include "Font.hpp"
#include "World.hpp"

ScriptManager::FileNotFoundError::FileNotFoundError(const std::string &filename)
        : message_(std::format("Lua script not found: {}", filename)) { }

const char *ScriptManager::FileNotFoundError::what() const noexcept {
    return message_.c_str();
}

ScriptManager::ScriptError::ScriptError(std::string &&error)
        : message_(std::format("Lua script error: {}", error)) { }

const char *ScriptManager::ScriptError::what() const noexcept {
    return message_.c_str();
}

ScriptManager::WrapperError::WrapperError(std::string &&error)
        : message_(std::format("Lua wrapper error: {}", error)) { }

const char *ScriptManager::WrapperError::what() const noexcept {
    return message_.c_str();
}

ScriptManager::ScriptManager() {
    L_ = luaL_newstate();

    lua_newtable(L_);
    lua_setglobal(L_, "Engine");
    Lua::Font::registerLua(L_);
    Lua::World::registerLua(L_);

    luaL_openlibs(L_);
}

ScriptManager::~ScriptManager() {
    lua_close(L_);
}

void ScriptManager::loadMainScript() {
    doFileImpl("game.lua");
}

void ScriptManager::initApiCall() {
    if (!getCallback("init"))
        return;
    if (lua_pcall(L_, 0, 0, 0) != LUA_OK)
        throwLuaError();
}

bool ScriptManager::getCallback(const char *name) {
    lua_getglobal(L_, "Engine");
    lua_getfield(L_, -1, name);
    if (lua_isnil(L_, -1)) {
        lua_pop(L_, lua_gettop(L_));
        return false;
    }
    return true;
}

void ScriptManager::throwLuaError() {
    std::string error(lua_tostring(L_, -1));
    lua_pop(L_, lua_gettop(L_));
    throw ScriptError(std::move(error));
}
