#include "ScriptManager.hpp"
#include <format>
#include "Font.hpp"
#include "World.hpp"
#include "Keyboard.hpp"
#include "../util/Logger.hpp"

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
    lua_pushvalue(L_, -1);
    lua_setglobal(L_, "Engine");
    Lua::Font::registerLua(L_);
    Lua::World::registerLua(L_);
    Lua::Keyboard::registerLua(L_);

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

void ScriptManager::timeStepApiCall(double time) {
    if (!getCallback("timeStep"))
        return;
    lua_pushnumber(L_, time);
    if (lua_pcall(L_, 1, 0, 0) != LUA_OK)
        throwLuaError();
}

void ScriptManager::keyPressedApiCall(const std::string &key) {
    if (!getCallback("Keyboard", "keyPressed"))
        return;
    lua_pushstring(L_, key.c_str());
    if (lua_pcall(L_, 1, 0, 0) != LUA_OK)
        throwLuaError();
}

void ScriptManager::keyReleasedApiCall(const std::string &key) {
    if (!getCallback("Keyboard", "keyReleased"))
        return;
    lua_pushstring(L_, key.c_str());
    if (lua_pcall(L_, 1, 0, 0) != LUA_OK)
        throwLuaError();
}

void ScriptManager::checkStack(int &top) {
    if (top == 1)
        return;
    const char *error = (top > 1) ? "Stack overflow" : "Stack empty";
    if (top != 1) { // If the engine table is not the only thing on the stack
        Log::warn("ScriptManager::checkStack(): Lua wrapper stack error: {}. Repairing the stack...", error);
        lua_settop(L_, 0);
        lua_getglobal(L_, "Engine");
        top = 1;
    }
}

bool ScriptManager::getCallback(const char *module, const char *name)
{
    int top = lua_gettop(L_);
    checkStack(top);
    lua_getfield(L_, -1, module);
    if (lua_isnil(L_, -1)) {
        lua_settop(L_, top);
        throw WrapperError(std::format("Could not find module: Engine.{}", module));
    }
    lua_getfield(L_, -1, name);
    if (lua_isnil(L_, -1)) {
        lua_settop(L_, top);
        return false;
    }
    lua_remove(L_, -2);
    return true;
}

bool ScriptManager::getCallback(const char *name)
{
    int top = lua_gettop(L_);
    checkStack(top);
    lua_getfield(L_, -1, name);
    if (lua_isnil(L_, -1)) {
        lua_settop(L_, top);
        return false;
    }
    return true;
}

void ScriptManager::throwLuaError() {
    std::string error(lua_tostring(L_, -1));
    lua_settop(L_, 1); // Keep only the Engine table on the stack
    throw ScriptError(std::move(error));
}
